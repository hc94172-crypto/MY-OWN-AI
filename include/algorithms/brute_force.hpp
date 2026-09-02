#ifndef BRUTE_FORCE_HPP
#define BRUTE_FORCE_HPP

#include "../models/vector_item.hpp"
#include "../utils/distance.hpp"

#include <vector>
#include <algorithm>
class BruteForce {
public:
    std::vector<VectorItem> items;

    void insert(const VectorItem& v);

    std::vector<std::pair<float,int>> knn(
        const std::vector<float>& q,
        int k,
        DistFn dist);

    void remove(int id);
};

#endif