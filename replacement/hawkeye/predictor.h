#ifndef PREDICTOR_H
#define PREDICTOR_H

#include <cstddef>
#include <cstdint>
#include <vector>

class HawkeyePredictor {
    public:
// num_entries: size of the PC-indexed table (paper: 8K entries)
// counter_bits: width of the saturating counter (paper: 3 bits, range [0, 2^counter_bits - 1])
    HawkeyePredictor(std::size_t num_entries = 8192, int counter_bits = 3);


// Trains the counter indexed by a hash of `pc` per Section 3.3's update rule.
    void train(uint64_t pc, bool opt_hit);


// Returns the predicted classification for `pc`.
    bool predict(uint64_t pc) const;


// Debug-only accessor: raw counter value in [0, 2^counter_bits - 1].
    int get_counter(uint64_t pc) const;

private:
    std::size_t index(uint64_t pc) const;

    std::size_t num_entries;
    int counter_bits;
    int max_val;
    int threshold;              //this is the boundary which decides the predictor decision, also used for initialization
    std::vector<int> table;
};

#endif