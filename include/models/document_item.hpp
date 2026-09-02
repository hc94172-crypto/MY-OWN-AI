#ifndef DOCUMENT_ITEM_HPP
#define DOCUMENT_ITEM_HPP

#include <string>
#include <vector>

struct DocumentItem
{
    int id;
    std::string title;
    std::string text;
    std::vector<float> emb;
};

#endif