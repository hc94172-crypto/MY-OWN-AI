#ifndef ROUTES_HPP
#define ROUTES_HPP

#include "../../third_party/httplib.h"

#include "../database/vector_db.hpp"
#include "../database/document_db.hpp"
#include "../network/ollama_client.hpp"

void registerRoutes(
    httplib::Server& svr,
    VectorDB& db,
    DocumentDB& docDB,
    OllamaClient& ollama
);

#endif
