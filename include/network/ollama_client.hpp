#ifndef OLLAMA_CLIENT_HPP
#define OLLAMA_CLIENT_HPP

#include "../../third_party/httplib.h"
#include "../utils/json_utils.hpp"

#include <string>
#include <vector>

// Despite the name (kept for compatibility with routes.cpp / main.cpp),
// this class now calls hosted free-tier APIs instead of a local Ollama:
//   - embed()    -> Google Gemini embeddings API
//   - generate() -> Groq chat completions API
class OllamaClient {
private:
    std::string groqKey;
    std::string geminiKey;

    std::string esc(const std::string& s);
    std::vector<float> parseEmbedding(const std::string& body);
    std::string parseResponse(const std::string& body);

public:
    // Kept as public fields so existing code (routes.cpp status endpoint etc.)
    // that prints ollama.embedModel / ollama.genModel still compiles.
    std::string embedModel = "gemini-embedding-001";
    std::string genModel   = "llama-3.1-8b-instant";

    OllamaClient();

    bool isAvailable();

    std::vector<float> embed(const std::string& text);

    std::string generate(const std::string& prompt);
};

#endif