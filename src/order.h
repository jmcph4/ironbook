#ifndef ORDER_H_
#define ORDER_H_

#include <stdbool.h>
#include <stdint.h>

#include "iron.h"
#include "trader.h"

typedef enum
{
    BID = 0,
    ASK = 1
} OrderType;

typedef struct
{
    uint64_t id;
    Trader* owner;
    OrderType type;
    long double price;
    uint64_t quantity;
} Order;

/* Initialisation */
iron_result_t order_init(uint64_t id, Trader* owner, OrderType type,
        long double price, uint64_t quantity, Order* order);
iron_result_t order_free(Order* order);

/* Access */
iron_result_t order_set_id(uint64_t id, Order* order);
iron_result_t order_get_id(uint64_t* id, Order* order);
iron_result_t order_set_owner(Trader* owner, Order* order);
iron_result_t order_get_owner(Trader** owner, Order* order);
iron_result_t order_set_type(OrderType type, Order* order);
iron_result_t order_get_type(OrderType* type, Order* order);
iron_result_t order_set_price(long double price, Order* order);
iron_result_t order_get_price(long double* price, Order* order);
iron_result_t order_set_quantity(uint64_t quantity, Order* order);
iron_result_t order_get_quantity(uint64_t* quantity, Order* order);

#endif /* ORDER_H_ */

