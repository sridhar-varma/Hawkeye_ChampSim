#include "hawkeye.h"

hawkeye::hawkeye(CACHE* cache)
    : champsim::modules::replacement(cache),
      optgen(cache->NUM_SET, cache->NUM_WAY, 8),
      predictor(8192, 3),
      rrpvs(cache->NUM_SET, std::vector<int>(cache->NUM_WAY, 7)) {}


long hawkeye::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set,
                          const champsim::cache_block* current_set, champsim::address ip,
                          champsim::address full_addr, access_type type){

    return static_cast<long>(::find_victim(rrpvs[static_cast<std::size_t>(set)]));
}



void hawkeye::replacement_cache_fill(uint32_t triggering_cpu, long set, long way,
                                     champsim::address full_addr, champsim::address ip,
                                     champsim::address victim_addr, access_type type){

    // here ip is pc that causes this line to be loaded
    // NOTE: ip=0 when this line comes from higher level caches due to eviction this plays very important role
    // as per repo [paper author's] when the cache line comes to LLC due to eviction from higher caches it is dealt differently 
    // In running benchmarks this particular point is effecting the percentage of improvement [In I got negative for some benchmark without this point]

    uint64_t pc = ip.to<uint64_t>();
    uint64_t block = full_addr.to<uint64_t>() >> 6;        // 64B lines
    std::size_t s = static_cast<std::size_t>(set);

    // must be read before access(), which rewrites the block's timestamp
    bool recent = optgen.is_in_history(s, block);
    bool opt_hit = optgen.access(s, block);

    // Section 3.3 trains the PC that last touched the block, not the current one, and
    // only when the reuse was inside the window - outside it OPTgen has no verdict because as per 8*associativity window this is first time access to this line
    auto it = last_pc.find(block);
    if(it != last_pc.end() && recent){
        predictor.train(it->second, opt_hit);
    }

    // this is where we use the above mentioned cavet when ip=0, here pc=ip are considered as CACHE_AVERSE
    bool friendly = (pc != 0) && predictor.predict(pc);
    update_rrpv(rrpvs[s], static_cast<std::size_t>(way),
                friendly ? Classification::CACHE_FRIENDLY : Classification::CACHE_AVERSE,
                false);

    if(pc != 0){
        last_pc[block] = pc;
    }
}


void hawkeye::update_replacement_state(uint32_t triggering_cpu, long set, long way,
                                       champsim::address full_addr, champsim::address ip,
                                       champsim::address victim_addr, access_type type,
                                       bool hit){

    if(!hit) return;    // fills are handled above

    // here we deal with only when cache hit occurs cache misses are dealt in above method.
    // there is no particular reason to do so except cleanliness
    uint64_t pc = ip.to<uint64_t>();
    uint64_t block = full_addr.to<uint64_t>() >> 6;
    std::size_t s = static_cast<std::size_t>(set);

    bool recent = optgen.is_in_history(s, block);
    bool opt_hit = optgen.access(s, block);

    auto it = last_pc.find(block);
    if(it != last_pc.end() && recent){
        predictor.train(it->second, opt_hit);
    }

    // Writebacks carry no PC. Classifying them gives averse, which would stamp a hot
    // line to 7, so leave the line's existing state alone.
    // pc!=0 guard for same reason mentioned in the above method
    if(pc != 0){
        bool friendly = predictor.predict(pc);
        update_rrpv(rrpvs[s], static_cast<std::size_t>(way),
                    friendly ? Classification::CACHE_FRIENDLY : Classification::CACHE_AVERSE,
                    true);
        last_pc[block] = pc;
    }
}