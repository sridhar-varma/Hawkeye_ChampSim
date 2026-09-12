#ifndef OPTGEN_H
#define OPTGEN_H

#include <cstddef>
#include <cstdint>
#include <unordered_map>
#include <vector>

class OPTgen {

    public:
// num_sets: number of cache sets tracked independently
// associativity: W, the cache associativity (occupancy vector cap)
// history_multiplier: length of tracked history, in units of the set's
// capacity (paper uses 8x; see Figure 2). Default 8.

        OPTgen(std::size_t num_sets, std::size_t associativity, std::size_t history_multiplier = 8);

    // Processes one access to `address`, mapped to set `set_idx`, per Section 3.1.
        bool access(std::size_t set_idx, uint64_t address);

    // -------------------- Upto Here this is untouched, From here these are add ons------------------------

    // Tells whether this block was last seen inside the 8W history window.
        bool is_in_history(std::size_t set_idx, uint64_t address) const;

    private:
        struct SetState{
            std::vector<int> occupancy_vector;
            std::unordered_map<uint64_t,uint64_t> last_access;
            uint64_t clock = 0;
        };

        std::size_t W;
        std::size_t window;     // 8*W
        std::vector<SetState> sets;
};

#endif