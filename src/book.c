#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include <jcrl/constants.h>
#include <jcrl/list.h>

#include "iron.h"
#include "book.h"

iron_result_t book_init(uint64_t id, char* name, List* traders, Book* book)
{
    if(name == NULL || traders == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    book->id = id;

    book->name = calloc(strlen(name) + 1, sizeof(char));

    if(name == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    book->traders = calloc(1, sizeof(List));

    if(traders == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    book->bids = calloc(1, sizeof(List));

    if(book->bids == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    book->asks = calloc(1, sizeof(List));

    if(book->asks == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    book->LTP = NAN;
    book->spread = 0.00;
    
    return IRON_ERR_OK;
}

iron_result_t book_free(Book* book)
{
    if(book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    free(book->name);
    free(book->traders);
    free(book->bids);
    free(book->asks);

    return IRON_ERR_OK;
}

iron_result_t book_set_id(uint64_t id, Book* book)
{
    if(book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    book->id = id;

    return IRON_ERR_OK;
}

iron_result_t book_get_id(uint64_t* id, Book* book)
{
    if(id == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *id = book->id;

    return IRON_ERR_OK;
}

iron_result_t book_set_name(char* name, Book* book)
{
    if(book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    strncpy(book->name, name, strlen(book->name));

    return IRON_ERR_OK;
}

iron_result_t book_get_name(char** name, Book* book)
{
    if(name == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *name = calloc(strlen(book->name), sizeof(char));

    if(*name == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    strncpy(*name, book->name, strlen(book->name));

    return IRON_ERR_OK;
}

iron_result_t book_add_trader(Trader* trader, Book* book)
{
    if(trader == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }
    
    unsigned int jcrl_result = list_append(trader, book->traders);

    if(jcrl_result != JCRL_ERR_OK)
    {
        return IRON_ERR_JCRL_FAILURE;
    }

    return IRON_ERR_OK;
}

iron_result_t book_remove_trader(uint64_t id, Book* book)
{
    if(book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    unsigned int num_traders = 0;
    unsigned int jcrl_result = list_length(&num_traders, book->traders);

    if(jcrl_result != JCRL_ERR_OK)
    {
        return IRON_ERR_JCRL_FAILURE;
    }

    bool found = false;
    Trader* curr_trader = NULL;

    for(unsigned int i=0;i<num_traders;i++)
    {
        jcrl_result = list_get((void**)&curr_trader, i, book->traders);

        if(jcrl_result != JCRL_ERR_OK)
        {
            return IRON_ERR_JCRL_FAILURE;
        }

        if(curr_trader->id == id)
        {
            jcrl_result = list_remove(i, book->traders);

            if(jcrl_result != JCRL_ERR_OK)
            {
                return IRON_ERR_JCRL_FAILURE;
            }

            found = true;
            break;
        }
    }

    /* TODO: remove all orders attached to the removed trader */
    
    if(!found)
    {
        return IRON_ERR_NOT_FOUND;
    }

    return IRON_ERR_OK;
}

iron_result_t book_submit(Order* order, Book* book)
{
    if(order == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    /* TODO: submit */

    return IRON_ERR_OK;
}

iron_result_t cancel(uint64_t order_id, Book* book)
{
    if(book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    order_id += 0; /* suppress unused warning */

    /* TODO: cancel */

    return IRON_ERR_OK;
}

iron_result_t LTP(long double* LTP, Book* book)
{
    if(LTP == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *LTP = book->LTP;

    return IRON_ERR_OK;
}

iron_result_t depth(uint64_t* bid, uint64_t* ask, Book* book)
{
    if(bid == NULL || ask == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    /* TODO: depth */

    return IRON_ERR_OK;
}

iron_result_t spread(long double* spread, Book* book)
{
    if(spread == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *spread = book->spread;

    return IRON_ERR_OK;
}

iron_result_t volume(uint64_t* bid, uint64_t* ask, Book* book)
{
    if(bid == NULL || ask == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    /* TODO: volume */

    return IRON_ERR_OK;
}

iron_result_t top(long double* bid, long double* ask, Book* book)
{
    if(bid == NULL || ask == NULL || book == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    /* TODO: top */

    return IRON_ERR_OK;
}


