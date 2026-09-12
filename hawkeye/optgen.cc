#include "optgen.h"

OPTgen::OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier)
    : W(associativity), window(associativity * history_multiplier), sets(num_sets) {

    for(auto& s : sets){
        s.occupancy_vector.assign(window, 0);
    }
}

bool OPTgen::access(std::size_t set_idx, uint64_t address){

    auto& s = sets[set_idx];
    uint64_t now = s.clock;

    // this mod access is to mimic the circular array data structure
    s.occupancy_vector[now % window] = 0;

    bool opt_hit = false;
    auto it = s.last_access.find(address);

    if(it != s.last_access.end()){

        uint64_t prev = it->second;

        // older than the window, treat as a first time load. This is adopted here because delete logic will make it more complex
        // but in reality this should actually the data structure should be either overwritten circularly or entry should be deleted as it exceeds window
        // it will be done via using valid bit as per repo
        if((now - prev) < window){

            bool fits = true;
            for(uint64_t t = prev; t < now; ++t){
                if(s.occupancy_vector[t % window] >= static_cast<int>(W)){
                    fits = false;
                    break;
                }
            }

            if(fits){
                opt_hit = true;
                for(uint64_t t = prev; t < now; ++t){
                    s.occupancy_vector[t % window]++;
                }
            }
            // a miss bypasses, so the vector is left alone this is as per paper
        }
    }

    s.last_access[address] = now;
    //increment the timestamp
    s.clock = now + 1;
    return opt_hit;
}


// this method has nothing to do with the 60 marks. This is written to be used by hawkeye.cc because this is effecting percentages in plots
bool OPTgen::is_in_history(std::size_t set_idx, uint64_t address) const{

    const auto& s = sets[set_idx];
    auto it = s.last_access.find(address);

    if(it == s.last_access.end()) return false;
    return (s.clock - it->second) < window;
}