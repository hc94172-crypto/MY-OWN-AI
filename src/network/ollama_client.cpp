#include "../../include/network/ollama_client.hpp"
#include <cstdlib>
#include <iostream>

std::string OllamaClient::esc(const std::string& s)
{
    std::string o;
    for (char c : s)
    {
        if (c == '"')       o += "\\\"";
        else if (c == '\\') o += "\\\\";
        else if (c == '\n') o += "\\n";
        else if (c == '\r') o += "\\r";
        else if (c == '\t') o += "\\t";
        else o += c;
    }
    return o;
}

std::vector<float> OllamaClient::parseEmbedding(const std::string& body)
{
    // Gemini response: {"embedding":{"values":[0.1,0.2,...]}}
    size_t p = body.find("\"embedding\"");
    if (p == std::string::npos) return {};

    p = body.find('[', p);
    if (p == std::string::npos) return {};

    size_t e = p + 1;
    int depth = 1;
    while (e < body.size() && depth > 0)
    {
        if (body[e] == '[') depth++;
        else if (body[e] == ']') depth--;
        e++;
    }
    return parseVec(body.substr(p + 1, e - p - 2));
}

std::string OllamaClient::parseResponse(const std::string& body)
{
    // Groq (OpenAI-style) response: {"choices":[{"message":{"content":"..."}}]}
    return extractStr(body, "content");
}

OllamaClient::OllamaClient()
{
    const char* g = std::getenv("GROQ_API_KEY");
    const char* gem = std::getenv("GEMINI_API_KEY");
    groqKey   = g   ? g   : "";
    geminiKey = gem ? gem : "";

    if (groqKey.empty())
        std::cout << "[WARN] GROQ_API_KEY not set" << std::endl;
    if (geminiKey.empty())
        std::cout << "[WARN] GEMINI_API_KEY not set" << std::endl;
}

bool OllamaClient::isAvailable()
{
    if (groqKey.empty() || geminiKey.empty()) return false;

    httplib::SSLClient cli("api.groq.com");
    cli.set_connection_timeout(5, 0);

    httplib::Headers headers = {
        { "Authorization", "Bearer " + groqKey }
    };
    auto res = cli.Get("/openai/v1/models", headers);
    return res && res->status == 200;
}

std::vector<float> OllamaClient::embed(const std::string& text)
{
    if (geminiKey.empty()) return {};

    httplib::SSLClient cli("generativelanguage.googleapis.com");
    cli.set_connection_timeout(10, 0);
    cli.set_read_timeout(60, 0);

    std::string path = "/v1beta/models/" + embedModel + ":embedContent?key=" + geminiKey;
    std::string body =
        "{\"content\":{\"parts\":[{\"text\":\"" + esc(text) + "\"}]}}";

    auto res = cli.Post(path.c_str(), body, "application/json");

    if (!res || res->status != 200) {
        if (res) std::cout << "[Gemini Error] HTTP " << res->status << ": " << res->body << std::endl;
        return {};
    }

    return parseEmbedding(res->body);
}

std::string OllamaClient::generate(const std::string& prompt)
{
    if (groqKey.empty())
        return "ERROR: GROQ_API_KEY not set on the server.";

    httplib::SSLClient cli("api.groq.com");
    cli.set_connection_timeout(10, 0);
    cli.set_read_timeout(60, 0);

    std::string body =
        "{\"model\":\"" + genModel + "\","
        "\"messages\":[{\"role\":\"user\",\"content\":\"" + esc(prompt) + "\"}]}";

    httplib::Headers headers = {
        { "Authorization", "Bearer " + groqKey }
    };

    auto res = cli.Post("/openai/v1/chat/completions", headers, body, "application/json");

    if (!res) {
        return "ERROR: Connection to Groq failed.";
    }
    if (res->status != 200) {
        std::cout << "[Groq Error] HTTP " << res->status << ": " << res->body << std::endl;
        return "ERROR: Groq returned HTTP " + std::to_string(res->status) + ": " + res->body;
    }

    return parseResponse(res->body);
}