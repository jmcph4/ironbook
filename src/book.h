#ifndef BOOK_H_
#define BOOK_H_

#include <stdbool.h>
#include <stdint.h>

#include <jcrl/list.h>

#include "iron.h"
#include "trader.h"
#include "order.h"

typedef struct
{
    uint64_t id;
    char* name;
    List* traders;
    List* bids;
    List* asks;
    long double LTP;
    long double spread;
} Book;

/* Initialisation */
iron_result_t book_init(uint64_t id, char* name, List* traders, Book* book);
iron_result_t book_free(Book* book);

/* Access */
iron_result_t book_set_id(uint64_t id, Book* book);
iron_result_t book_get_id(uint64_t* id, Book* book);
iron_result_t book_set_name(char* name, Book* book);
iron_result_t book_get_name(char** name, Book* book);

/* Equality */
iron_result_t book_equal(Book* a, Book* b, bool* equal);

/* Operations */
iron_result_t book_add_trader(Trader* trader, Book* book);
iron_result_t book_remove_trader(uint64_t id, Book* book);
iron_result_t book_submit(Order* order, Book* book);
iron_result_t book_cancel(uint64_t order_id, Book* book);
iron_result_t book_LTP(long double* LTP, Book* book);
iron_result_t book_depth(uint64_t* bid, uint64_t* ask, Book* book);
iron_result_t book_spread(long double* spread, Book* book);
iron_result_t book_volume(uint64_t* bid, uint64_t* ask, Book* book);
iron_result_t book_top(long double* bid, long double* ask, Book* book);

#endif /* BOOK_H_ */

