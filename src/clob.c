#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "clob.h"

ssize_t level_insert(struct level* level, struct order order) {
    if (level == NULL || level->price != order.price) return -1;
    struct order* new_orders =
        realloc(level->orders, (level->num_orders + 1) * sizeof(struct order));
    if (new_orders == NULL) return -1;
    level->orders = new_orders;
    level->num_orders++;
    level->orders[level->num_orders - 1] = order;
    return level->num_orders;
}

ssize_t level_pos(const struct level* level, oid_t order_id) {
    if (level == NULL || level->num_orders == 0) return -1;
    for (size_t i = 0; i < level->num_orders; i++)
        if (level->orders[i].id == order_id) return i;
    return -1;
}

ssize_t level_remove_at(struct level* level, size_t pos) {
    if (level == NULL || pos >= level->num_orders) return -1;
    for (size_t i = pos; i < level->num_orders - 1; i++)
        level->orders[i] = level->orders[i + 1];
    level->num_orders--;
    return (ssize_t)(level->num_orders);
}

ssize_t level_remove_by_oid(struct level* level, oid_t order_id) {
    if (level == NULL) return -1;
    ssize_t pos = level_pos(level, order_id);
    if (pos == -1) return -1;
    return level_remove_at(level, (size_t)pos);
}

struct level* book_find_level_by_price(struct level** levels, size_t num_levels,
                                       uint64_t price) {
    if (levels == NULL || num_levels == 0) return NULL;
    for (size_t i = 0; i < num_levels; i++)
        if (levels[i]->price == price) return levels[i];
    return NULL;
}

void book_delete_level(struct book* book, enum side side, size_t loff) {
    if (book == NULL) return;
    switch (side) {
    case BID: {
        if (loff >= book->num_bid_levels) return;
        struct level* to_delete = book->bids[loff];
        free(to_delete->orders);
        to_delete->num_orders = 0;
        free(to_delete);
        for (size_t i = loff; i < book->num_bid_levels - 1; i++)
            book->bids[i] = book->bids[i + 1];
        book->num_bid_levels--;
        break;
    }
    case ASK: {
        if (loff >= book->num_ask_levels) return;
        struct level* to_delete = book->asks[loff];
        free(to_delete->orders);
        to_delete->num_orders = 0;
        free(to_delete);
        for (size_t i = loff; i < book->num_ask_levels - 1; i++)
            book->asks[i] = book->asks[i + 1];
        book->num_ask_levels--;
        break;
    }
    }
}

void book_submit(struct book* book, struct order order) {
    if (book == NULL || order.quantity == 0) return;

    uint64_t remaining_quantity = order.quantity;

    switch (order.side) {
    case BID: {
        if (book->num_ask_levels > 0) {
            uint64_t opposing_price = book->asks[0]->price;

            if (order.price >= opposing_price) {
                size_t curr_loff = 0;
                while (remaining_quantity > 0) {
                    if (curr_loff < book->num_ask_levels) {
                        struct level* curr_level = book->asks[curr_loff];
                        if (curr_level->price > order.price) goto post_order;
                        for (size_t i = 0; i < curr_level->num_orders; i++) {
                            struct order* curr_order = &curr_level->orders[i];
                            if (curr_order->quantity > remaining_quantity) {
                                curr_order->quantity -= remaining_quantity;
                                remaining_quantity = 0;
                                return;
                            } else if (curr_order->quantity ==
                                       remaining_quantity) {
                                curr_order->quantity -= remaining_quantity;
                                remaining_quantity = 0;
                                level_remove_at(curr_level, i);
                                if (curr_level->num_orders == 0)
                                    book_delete_level(book, ASK, curr_loff);
                                return;
                            } else {
                                remaining_quantity -= curr_order->quantity;
                                curr_order->quantity = 0;
                                level_remove_at(curr_level, i);
                                i--; /* adjust for the above deletion */
                                if (curr_level->num_orders == 0) {
                                    book_delete_level(book, ASK, curr_loff);
                                    curr_loff--; /* adjust for later increment
                                                    as we've just deleted this
                                                    level */
                                    break;
                                }
                            }
                        }
                        if (remaining_quantity > 0) curr_loff++;
                    } else
                        goto post_order;
                }
            } else
                goto post_order;
        } else
            goto post_order;
        break;
    }
    case ASK: {
        if (book->num_bid_levels > 0) {
            uint64_t opposing_price = book->bids[0]->price;

            if (order.price <= opposing_price) {
                size_t curr_loff = 0;
                while (remaining_quantity > 0) {
                    if (curr_loff < book->num_bid_levels) {
                        struct level* curr_level = book->bids[curr_loff];
                        if (curr_level->price < order.price) goto post_order;
                        for (size_t i = 0; i < curr_level->num_orders; i++) {
                            struct order* curr_order = &curr_level->orders[i];
                            if (curr_order->quantity > remaining_quantity) {
                                curr_order->quantity -= remaining_quantity;
                                remaining_quantity = 0;
                                return;
                            } else if (curr_order->quantity ==
                                       remaining_quantity) {
                                curr_order->quantity -= remaining_quantity;
                                remaining_quantity = 0;
                                level_remove_at(curr_level, i);
                                if (curr_level->num_orders == 0)
                                    book_delete_level(book, BID, curr_loff);
                                return;
                            } else {
                                remaining_quantity -= curr_order->quantity;
                                curr_order->quantity = 0;
                                level_remove_at(curr_level, i);
                                i--; /* adjust for the above deletion */
                                if (curr_level->num_orders == 0) {
                                    book_delete_level(book, BID, curr_loff);
                                    curr_loff--; /* adjust for later increment
                                                    as we've just deleted this
                                                    level */
                                    break;
                                }
                            }
                        }
                        if (remaining_quantity > 0) curr_loff++;
                    } else
                        goto post_order;
                }
            } else
                goto post_order;
        } else
            goto post_order;
        break;
    }
    }
    /* no need to check for remaining as we handle that within the internal
     * matching loop */
    return;

post_order:
    order.quantity = remaining_quantity;
    book_insert(book, order);
    return;
}

void book_cancel(struct book* book, oid_t order_id) {
    if (book == NULL) return;
    book_remove(book, order_id);
}

ssize_t book_new_level_offset(struct book* book, uint64_t price,
                              enum side side) {
    if (book == NULL) return -1;
    switch (side) {
    case BID: {
        if (book->num_bid_levels == 0) return 0;
        for (size_t i = 0; i < book->num_bid_levels; i++) {
            struct level* curr_level = book->bids[i];
            if (curr_level->price == price || curr_level->price < price)
                return i;
        }
        return book->num_bid_levels;
    }
    case ASK: {
        if (book->num_ask_levels == 0) return 0;
        for (size_t i = 0; i < book->num_ask_levels; i++) {
            struct level* curr_level = book->asks[i];
            if (curr_level->price == price) return i;
            if (curr_level->price == price || curr_level->price > price)
                return i;
        }
        return book->num_ask_levels;
    }
    }
    return 0;
}

void book_insert(struct book* book, struct order order) {
    if (book == NULL) return;

    switch (order.side) {
    case BID: {
        struct level* level = book_find_level_by_price(
            book->bids, book->num_bid_levels, order.price);
        if (level == NULL) {
            size_t new_pos =
                book_new_level_offset(book, order.price, order.side);
            struct level** new_bids = realloc(
                book->bids, (book->num_bid_levels + 1) * sizeof(struct level*));
            if (new_bids == NULL) return;
            book->bids = new_bids;
            book->num_bid_levels++;
            for (size_t i = book->num_bid_levels - 1; i > new_pos; i--)
                book->bids[i] = book->bids[i - 1];
            book->bids[new_pos] = calloc(1, sizeof(struct level));
            book->bids[new_pos]->price = order.price;
            book_insert(book, order);
        } else {
            level_insert(level, order);
        }
        break;
    }
    case ASK: {
        struct level* level = book_find_level_by_price(
            book->asks, book->num_ask_levels, order.price);
        if (level == NULL) {
            size_t new_pos =
                book_new_level_offset(book, order.price, order.side);
            struct level** new_asks = realloc(
                book->asks, (book->num_ask_levels + 1) * sizeof(struct level*));
            if (new_asks == NULL) return;
            book->asks = new_asks;
            book->num_ask_levels++;
            for (size_t i = book->num_ask_levels - 1; i > new_pos; i--)
                book->asks[i] = book->asks[i - 1];
            book->asks[new_pos] = calloc(1, sizeof(struct level));
            book->asks[new_pos]->price = order.price;
            book_insert(book, order);
        } else {
            level_insert(level, order);
        }
        break;
    }
    }
}

void book_remove(struct book* book, oid_t order_id) {
    if (book == NULL) return;
    for (size_t i = 0; i < book->num_bid_levels; i++) {
        ssize_t new_num_orders = level_remove_by_oid(book->bids[i], order_id);
        if (new_num_orders == 0) book_delete_level(book, BID, i);
    }
    for (size_t i = 0; i < book->num_ask_levels; i++) {
        ssize_t new_num_orders = level_remove_by_oid(book->asks[i], order_id);
        if (new_num_orders == 0) book_delete_level(book, ASK, i);
    }
}

struct book* book_init() {
    struct book* book = calloc(1, sizeof(struct book));
    if (book == NULL) return NULL;
    return book;
}

void book_free(struct book* book) {
    if (book == NULL) return;

    if (book->bids != NULL) {
        for (size_t i = 0; i < book->num_bid_levels; i++)
            level_free(book->bids[i]);
        free(book->bids);
    }

    if (book->asks != NULL) {
        for (size_t i = 0; i < book->num_ask_levels; i++)
            level_free(book->asks[i]);
        free(book->asks);
    }
    free(book);
}

void book_print_levels(struct level** levels, size_t num_levels) {
    if (levels == NULL || num_levels == 0) return;
    for (size_t i = 0; i < num_levels; i++) {
        struct level* curr_level = levels[i];
        printf("%ld: ", curr_level->price);
        for (size_t j = 0; j < curr_level->num_orders; j++)
            printf("%ld ", curr_level->orders[j].quantity);
        printf("\n");
    }
}

void book_print(struct book* book) {
    if (book == NULL) return;
    book_print_levels(book->asks, book->num_ask_levels);
    book_print_levels(book->bids, book->num_bid_levels);
}

void level_free(struct level* level) {
    if (level == NULL) return;
    if (level->orders != NULL) free(level->orders);
    free(level);
}
