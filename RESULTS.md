# Hawkeye Cache Replacement — Results

## Plot 1 — LLC miss rate vs. associativity (456.hmmer-191B)

| Ways | Sets | LRU miss rate | Hawkeye miss rate |
|-----:|-----:|--------------:|------------------:|
| 4    | 8192 | 33.89%        | 34.59%            |
| 8    | 4096 | 26.93%        | 28.00%            |
| 16   | 2048 | 25.94%        | 25.30%            |

## Plot 2 — LLC miss-rate reduction over LRU (2048 sets x 16 ways)

| Benchmark        | LRU miss rate | Hawkeye miss rate | Reduction |
|------------------|--------------:|------------------:|----------:|
| 456.hmmer-191B   | 25.94%        | 25.30%            | +2.48%    |
| 429.mcf-22B      | 64.82%        | 64.59%            | +0.36%    |
| 473.astar-42B    | 19.63%        | 19.50%            | +0.68%    |
