#ifndef DOCUMENT_DB_HPP
#define DOCUMENT_DB_HPP

#include "../models/vector_item.hpp"
#include "../models/document_item.hpp"

#include "../algorithms/brute_force.hpp"
#include "../algorithms/hnsw.hpp"

#include <unordered_map>
#include <vector>
#include <string>
#include <mutex>

class DocumentDB {
private:
    std::unordered_map<int, DocumentItem> store;

    HNSW hnsw;
    BruteForce bf;

    std::mutex mu;

    int nextId = 1;
    int dims = 0;

public:
    DocumentDB();

    int insert(const std::string& title,
               const std::string& text,
               const std::vector<float>& emb);

    std::vector<std::pair<float, DocumentItem>> search(
        const std::vector<float>& q,
        int k,
        float max_dist = 0.7f);

    bool remove(int id);

    std::vector<DocumentItem> all();

    size_t size();

    int getDims();
};

#endif