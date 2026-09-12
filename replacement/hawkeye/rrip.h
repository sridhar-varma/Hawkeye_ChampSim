#ifndef RRIP_H
#define RRIP_H

#include <cstddef>
#include <vector>


enum class Classification { CACHE_FRIENDLY, CACHE_AVERSE };
// Applies Table 1's update rule to each set's RRPV vector.
void update_rrpv(std::vector<int>& rrpv, std::size_t way, Classification cls, bool is_hit);
// Selects a victim way, aging the set if necessary, per Section 3.4.
std::size_t find_victim(std::vector<int>& rrpv);


#endif