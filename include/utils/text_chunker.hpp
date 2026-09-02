#ifndef TEXT_CHUNKER_HPP
#define TEXT_CHUNKER_HPP

#include <string>
#include <vector>

std::vector<std::string> chunkText(
    const std::string& text,
    int chunkWords = 250,
    int overlapWords = 30);

#endif