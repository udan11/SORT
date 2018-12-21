#!/bin/bash

# ./experiments --count    50000 --runs 1 --experiment bubble
# ./experiments --count   160000 --runs 1 --experiment insertion
# ./experiments --count  9000000 --runs 1 --experiment merge
# ./experiments --count 35000000 --runs 1 --experiment quick
# ./experiments --count 33000000 --runs 1 --experiment tim

# ---

# ./experiments --count   50000 --cpus 1 --runs 1 --experiment bubble
# ./experiments --count   50000 --cpus 1 --runs 1 --experiment insertion

# ./experiments --count 1000000 --cpus 1 --runs 1 --experiment merge
# ./experiments --count 1000000 --cpus 1 --runs 1 --experiment quick
# ./experiments --count 1000000 --cpus 1 --runs 1 --experiment tim

# for cpus in 1 2 4 8 12 16 20 24 28 32 36 40; do
#     ./experiments --count   50000 --cpus 4 --runs 1 --experiment bubble_omp
# done

# for cpus in 1 2 4 8 12 16 20 24 28 32 36 40; do
#     ./experiments --count 1000000 --cpus 4 --runs 1 --experiment merge_omp
#     ./experiments --count 1000000 --cpus 4 --runs 1 --experiment quick_omp
#     ./experiments --count 1000000 --cpus 4 --runs 1 --experiment tim_omp
# done

# ---

for cpus in 1 2 4 8 12 16 20 24 28 32 36 40; do
    ./benchmark --count 100000000 --cpus $cpus --tbb --pp --gpu
done
