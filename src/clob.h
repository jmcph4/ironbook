#ifndef CLOB_H_
#define CLOB_H_

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

typedef uint64_t oid_t;

enum side {
    BID,
    ASK,
};

struct order {
    oid_t id;
    enum side side;
    uint64_t price;
    uint64_t quantity;
};

struct level {
    uint64_t price;
    size_t num_orders;
    struct order* orders;
};

void level_free(struct level*);

ssize_t level_insert(struct level* level, struct order);
ssize_t level_remove(struct level* level, oid_t);
struct order* level_find(const struct level*, oid_t);

struct book {
    size_t num_bid_levels;
    struct level** bids;
    size_t num_ask_levels;
    struct level** asks;
};

struct book* book_init();
void book_free(struct book*);

void book_print(struct book*);

void book_insert(struct book*, struct order);
void book_remove(struct book*, oid_t);

void book_submit(struct book*, struct order);
void book_cancel(struct book*, oid_t);

#endif /* CLOB_H_ */

