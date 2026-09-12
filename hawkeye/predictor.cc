#include "predictor.h"

HawkeyePredictor::HawkeyePredictor(std::size_t num_entries, int counter_bits)
    : num_entries(num_entries),
      counter_bits(counter_bits),
      max_val((1 << counter_bits) - 1),
      threshold(1 << (counter_bits - 1)) {

    // start at the midpoint so an unseen PC comes out cache-friendly
    table.resize(num_entries, threshold);
}

std::size_t HawkeyePredictor::index(uint64_t pc) const{

    // as mentioned in assignment. I looked at CRC2 repo of hawkeye [like the authors repo actually] they followed different hash function
    // as per that repo it is 
    return (pc ^ (pc >> 12)) % num_entries;
}

void HawkeyePredictor::train(uint64_t pc, bool opt_hit){

    std::size_t i = index(pc);

    if(opt_hit){
        if(table[i] < max_val) table[i]++;
    }else{
        if(table[i] > 0) table[i]--;
    }
}

bool HawkeyePredictor::predict(uint64_t pc) const{

    // high order bit of the counter is the friendly/averse bit
    return table[index(pc)] >= threshold;
}

int HawkeyePredictor::get_counter(uint64_t pc) const{
    return table[index(pc)];
}