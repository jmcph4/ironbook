#ifndef TRADER_H_
#define TRADER_H_

#include <stdbool.h>
#include <stdint.h>

#include "iron.h"

typedef struct
{
    uint64_t id;
    char* name;
    long double bal;
    uint64_t vol;
} Trader;

/* Initialisation */
iron_result_t trader_init(uint64_t id, char* name, long double bal,
        uint64_t vol, Trader* trader);
iron_result_t trader_free(Trader* trader);

/* Access */
iron_result_t trader_set_id(uint64_t id, Trader* trader);
iron_result_t trader_get_id(uint64_t* id, Trader* trader);
iron_result_t trader_set_name(char* name, Trader* trader);
iron_result_t trader_get_name(char** name, Trader* trader);
iron_result_t trader_set_bal(long double bal, Trader* trader);
iron_result_t trader_get_bal(long double* bal, Trader* trader);
iron_result_t trader_set_vol(uint64_t vol, Trader* trader);
iron_result_t trader_get_vol(uint64_t* vol, Trader* trader);

#endif /* TRADER_H_ */

