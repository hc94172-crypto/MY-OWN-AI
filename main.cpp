#include "third_party/httplib.h"
#include "include/utils/distance.hpp"
#include "include/algorithms/brute_force.hpp"
#include "include/models/vector_item.hpp"
#include "include/algorithms/kd_tree.hpp"
#include "include/algorithms/hnsw.hpp"
#include "include/database/vector_db.hpp"
#include "include/utils/json_utils.hpp"
#include "include/utils/text_chunker.hpp"
#include "include/network/ollama_client.hpp"
#include "include/database/document_db.hpp"
#include "include/data/demo_data.hpp"
#include "include/api/routes.hpp"



#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <queue>
#include <set>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <climits>
#include <thread>
#include <condition_variable>
#include <cstdlib>



int main() {
    VectorDB   db(DIMS);
    DocumentDB docDB;
    OllamaClient ollama;

    const char* portEnv = std::getenv("PORT");
    int port = portEnv ? std::stoi(portEnv) : 8080;

    loadDemo(db);

    // Check availability at startup (non-fatal)
    bool llmUp = ollama.isAvailable();
    std::cout << "=== VectorDB Engine ===" << std::endl;
    std::cout << "Listening on port " << port << std::endl;
    std::cout << db.size() << " demo vectors | " << DIMS << " dims | HNSW+KD-Tree+BruteForce" << std::endl;
    std::cout << "LLM backend: " << (llmUp ? "ONLINE" : "OFFLINE (check GROQ_API_KEY / GEMINI_API_KEY)") << std::endl;
    if (llmUp) std::cout << "  embed model: " << ollama.embedModel
                         << "  gen model: "   << ollama.genModel << std::endl;

    httplib::Server svr;

    registerRoutes(svr, db, docDB, ollama);
    // CORS preflight
    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) {
        cors(res); res.status = 204;
    });

    svr.listen("0.0.0.0", port);
    return 0;
}