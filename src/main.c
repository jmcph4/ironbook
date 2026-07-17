#include <stdio.h>
#include <stdlib.h>

#include "clob.h"

#define NUM_ORDERS 5

int main(void) {
    struct book* foo = book_init();
    struct order orders[NUM_ORDERS] = {
        { .id = 1, .side = BID, .price = 12, .quantity = 33 },
        { .id = 2, .side = BID, .price = 12, .quantity = 33 },
        { .id = 3, .side = BID, .price = 11, .quantity = 200 },
        { .id = 4, .side = ASK, .price = 14, .quantity = 100 },
        { .id = 5, .side = ASK, .price = 12, .quantity = 100 }
    };

    book_print(foo);
    printf("===\n");

    for (size_t i=0;i<NUM_ORDERS;i++) {
        book_submit(foo, orders[i]);
        book_print(foo);
        printf("===\n");
    }

    book_free(foo);
    return EXIT_SUCCESS;
}

