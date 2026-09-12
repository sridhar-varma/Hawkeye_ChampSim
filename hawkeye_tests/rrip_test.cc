// rrip_test.cc
#include "../replacement/hawkeye/rrip.h"
#include <iostream>
#include <vector>
#include <cstddef>
int main()
{
    // --- Case 1: insertion policy ---
    std::vector<int> rrpv = {
    // left empty; the grading script substitutes initial per-way RRPV state
    };
    update_rrpv(rrpv, 0, Classification::CACHE_AVERSE, /*is_hit=*/false);
    update_rrpv(rrpv, 1, Classification::CACHE_FRIENDLY, /*is_hit=*/false);
    for (int v : rrpv)
        std::cout << v << " ";
    std::cout << "\n";
    std::cout << "victim: " << find_victim(rrpv) << "\n";
    // --- Case 2: find a victim ---
    std::vector<int> rrpv2 = {
    // left empty; the grading script substitutes initial per-way RRPV state
    };
    std::size_t v = find_victim(rrpv2);
    for (int x : rrpv2)
        std::cout << x << " ";
    std::cout << "\n";
    std::cout << "victim: " << v << "\n";
}