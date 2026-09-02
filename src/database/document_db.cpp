#include "../../include/database/document_db.hpp"
#include "../../include/utils/distance.hpp"

DocumentDB::DocumentDB()
    : hnsw(16, 200)
{
}

int DocumentDB::insert(
    const std::string& title,
    const std::string& text,
    const std::vector<float>& emb)
{
    std::lock_guard<std::mutex> lk(mu);

    if (dims == 0)
        dims = (int)emb.size();

    DocumentItem item{
        nextId++,
        title,
        text,
        emb
    };

    store[item.id] = item;

    VectorItem vi{
        item.id,
        item.title,
        "doc",
        item.emb
    };

    hnsw.insert(vi, cosine);
    bf.insert(vi);

    return item.id;
}

std::vector<std::pair<float, DocumentItem>>
DocumentDB::search(
    const std::vector<float>& q,
    int k,
    float max_dist)
{
    std::lock_guard<std::mutex> lk(mu);

    if (store.empty())
        return {};

    auto raw =
        (store.size() < 10)
        ? bf.knn(q, k, cosine)
        : hnsw.knn(q, k, 50, cosine);

    std::vector<std::pair<float, DocumentItem>> out;

    for (auto& [d, id] : raw)
    {
        if (store.count(id) && d <= max_dist)
            out.push_back({d, store[id]});
    }

    return out;
}

bool DocumentDB::remove(int id)
{
    std::lock_guard<std::mutex> lk(mu);

    if (!store.count(id))
        return false;

    store.erase(id);

    hnsw.remove(id);
    bf.remove(id);

    return true;
}

std::vector<DocumentItem> DocumentDB::all()
{
    std::lock_guard<std::mutex> lk(mu);

    std::vector<DocumentItem> r;

    for (auto& [id, v] : store)
        r.push_back(v);

    return r;
}

size_t DocumentDB::size()
{
    std::lock_guard<std::mutex> lk(mu);

    return store.size();
}

int DocumentDB::getDims()
{
    return dims;
}