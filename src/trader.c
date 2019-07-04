#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "iron.h"
#include "trader.h"

iron_result_t trader_init(uint64_t id, char* name, long double bal,
        uint64_t vol, Trader* trader)
{
    if(name == NULL || trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    trader->id = id;

    trader->name = calloc(strlen(name) + 1, sizeof(char));

    if(name == NULL) /* allocation check */
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    strncpy(trader->name, name, strlen(name));

    trader->bal = bal;
    trader->vol = vol;

    return IRON_ERR_OK;
}

iron_result_t trader_free(Trader* trader)
{
    if(trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    free(trader->name);

    return IRON_ERR_OK;
}

iron_result_t trader_set_id(uint64_t id, Trader* trader)
{
    if(trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    trader->id = id;

    return IRON_ERR_OK;
}

iron_result_t trader_get_id(uint64_t* id, Trader* trader)
{
    if(id == NULL || trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *id = trader->id;

    return IRON_ERR_OK;
}

iron_result_t trader_set_name(char* name, Trader* trader)
{
    if(trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    trader->name = name;

    return IRON_ERR_OK;
}

iron_result_t trader_get_name(char** name, Trader* trader)
{
    if(name == NULL || trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *name = calloc(strlen(trader->name) + 1, sizeof(char));

    if(*name == NULL)
    {
        return IRON_ERR_ALLOC_FAILURE;
    }

    strncpy(*name, trader->name, strlen(trader->name));

    return IRON_ERR_OK;
}

iron_result_t trader_set_bal(long double bal, Trader* trader)
{
    if(trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    trader->bal = bal;

    return IRON_ERR_OK;
}

iron_result_t trader_get_bal(long double* bal, Trader* trader)
{
    if(bal == NULL || trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *bal = trader->bal;

    return IRON_ERR_OK;
}

iron_result_t trader_set_vol(uint64_t vol, Trader* trader)
{
    if(trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    trader->vol = vol;

    return IRON_ERR_OK;
}

iron_result_t trader_get_vol(uint64_t* vol, Trader* trader)
{
    if(vol == NULL || trader == NULL) /* null guard */
    {
        return IRON_ERR_NULL_PARAM;
    }

    *vol = trader->vol;

    return IRON_ERR_OK;
}

