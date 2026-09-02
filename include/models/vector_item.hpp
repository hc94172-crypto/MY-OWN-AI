#ifndef VECTOR_ITEM_HPP
#define VECTOR_ITEM_HPP

#include <string>
#include <vector>

struct VectorItem {
    int id;
    std::string metadata;
    std::string category;
    std::vector<float> emb;
};

#endif