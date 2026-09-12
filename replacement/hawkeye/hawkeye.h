#ifndef HAWKEYE_H
#define HAWKEYE_H

#include <cstdint>
#include <unordered_map>
#include <vector>

#include "address.h"
#include "cache.h"
#include "modules.h"
#include "optgen.h"
#include "predictor.h"
#include "rrip.h"

struct hawkeye : public champsim::modules::replacement {

    OPTgen optgen;
    HawkeyePredictor predictor;
    std::vector<std::vector<int>> rrpvs;
    std::unordered_map<uint64_t,uint64_t> last_pc;   // block address -> PC of its last access

    hawkeye(CACHE* cache);

    // These must use champsim::address / access_type / long. cache.h selects an
    // overload with if constexpr concept checks and silently skips the module when
    // nothing matches, so with raw uint64_t params the module is never called on a miss.

    long find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set,
                     const champsim::cache_block* current_set, champsim::address ip,
                     champsim::address full_addr, access_type type);

    void replacement_cache_fill(uint32_t triggering_cpu, long set, long way,
                                champsim::address full_addr, champsim::address ip,
                                champsim::address victim_addr, access_type type);

    void update_replacement_state(uint32_t triggering_cpu, long set, long way,
                                  champsim::address full_addr, champsim::address ip,
                                  champsim::address victim_addr, access_type type,
                                  bool hit);
};

#endif