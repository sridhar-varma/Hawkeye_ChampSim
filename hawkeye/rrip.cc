#include "rrip.h"

#include <algorithm>

constexpr int MAX_RRPV = 7;

void update_rrpv(std::vector<int>& rrpv, std::size_t way, Classification cls, bool is_hit){

    // averse lines go to 7 on both hits and misses
    if(cls == Classification::CACHE_AVERSE){
        rrpv[way] = MAX_RRPV;
        return;
    }

    // friendly lines go to 0 on both, and a new insertion ages the rest of the set.
    // The reference checks first whether any line has already reached 6 - if one has,
    // nothing ages this time round.
    if(!is_hit){
        bool saturated = false;
        for(std::size_t i = 0; i < rrpv.size(); ++i){
            if(rrpv[i] == MAX_RRPV - 1){
                saturated = true;
                break;
            }
        }

        if(!saturated){
            for(std::size_t i = 0; i < rrpv.size(); ++i){
                if(i != way && rrpv[i] < MAX_RRPV - 1){
                    rrpv[i]++;
                }
            }
        }
    }
    rrpv[way] = 0;
}


std::size_t find_victim(std::vector<int>& rrpv){

    if(rrpv.empty()) return 0;

    for(std::size_t i = 0; i < rrpv.size(); i++){
        if(rrpv[i] == MAX_RRPV) return i;
    }

    // no averse line, so evict the oldest cache-friendly one. 
    std::size_t victim = 0;
    int highest = -1;
    for(std::size_t i = 0; i < rrpv.size(); i++){
        if(rrpv[i] >= highest){
            highest = rrpv[i];
            victim = i;
        }
    }
    return victim;
}