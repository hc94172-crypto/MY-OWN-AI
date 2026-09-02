#ifndef VECTOR_DB_HPP
#define VECTOR_DB_HPP

#include "../models/vector_item.hpp"
#include "../utils/distance.hpp"
#include "../algorithms/brute_force.hpp"
#include "../algorithms/kd_tree.hpp"
#include "../algorithms/hnsw.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>
#include <chrono>

class VectorDB
{
private:
    std::unordered_map<int, VectorItem> store;
    BruteForce bf;
    KDTree kdt;
    HNSW hnsw;
    std::mutex mu;
    int nextId = 1;

public:
    const int dims;

    explicit VectorDB(int d);

    struct Hit
    {
        int id;
        std::string meta;
        std::string cat;
        std::vector<float> emb;
        float dist;
    };

    struct SearchOut
    {
        std::vector<Hit> hits;
        long long us;
        std::string algo;
        std::string metric;
    };

    struct BenchOut
    {
        long long bfUs;
        long long kdUs;
        long long hnswUs;
        int n;
    };

    int insert(
        const std::string& meta,
        const std::string& cat,
        const std::vector<float>& emb,
        DistFn dist);

    bool remove(int id);

    SearchOut search(
        const std::vector<float>& q,
        int k,
        const std::string& metric,
        const std::string& algo);

    BenchOut benchmark(
        const std::vector<float>& q,
        int k,
        const std::string& metric);

    std::vector<VectorItem> all();

    HNSW::GraphInfo hnswInfo();

    size_t size();
};

#endif