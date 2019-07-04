#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "iron.h"
#include "order.h"

iron_result_t order_init(uint64_t id, Trader* owner, OrderType type,
        long double price, uint64_t quantity, Order* order)
{
    if(owner == NULL || order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    order->id = id;

    order->owner = calloc(1, sizeof(Trader));

    if(owner == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    memcpy(order->owner, owner, sizeof(Trader));

    order->type = type;
    order->price = price;
    order->quantity = quantity;

    return IRON_ERR_OK;
}

iron_result_t order_free(Order* order)
{
    if(order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    free(order->owner);

    return IRON_ERR_OK;
}

iron_result_t order_set_id(uint64_t id, Order* order)
{
    if(order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    order->id = id;

    return IRON_ERR_OK;
}

iron_result_t order_get_id(uint64_t* id, Order* order)
{
    if(id == NULL || order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *id = order->id;

    return IRON_ERR_OK;
}

iron_result_t order_set_owner(Trader* owner, Order* order)
{
    if(order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    memcpy(order->owner, owner, sizeof(Trader));

    return IRON_ERR_OK;
}

iron_result_t order_get_owner(Trader** owner, Order* order)
{
    if(owner == NULL || order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *owner = calloc(1, sizeof(Trader));

    if(*owner == NULL)
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    memcpy(*owner, order->owner, sizeof(Trader));

    return IRON_ERR_OK;
}

iron_result_t order_set_type(OrderType type, Order* order)
{
    if(order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    order->type = type;

    return IRON_ERR_OK;
}

iron_result_t order_get_type(OrderType* type, Order* order)
{
    if(type == NULL || order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *type = order->type;

    return IRON_ERR_OK;
}

iron_result_t order_set_price(long double price, Order* order)
{
    if(order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    order->price = price;

    return IRON_ERR_OK;
}

iron_result_t order_get_price(long double* price, Order* order)
{
    if(price == NULL || order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *price = order->price;

    return IRON_ERR_OK;
}

iron_result_t order_set_quantity(uint64_t quantity, Order* order)
{
    if(order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    order->quantity = quantity;

    return IRON_ERR_OK;
}

iron_result_t order_get_quantity(uint64_t* quantity, Order* order)
{
    if(quantity == NULL || order == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *quantity = order->quantity;

    return IRON_ERR_OK;
}

