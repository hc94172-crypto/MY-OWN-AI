#include "../../include/utils/json_utils.hpp"

#include <sstream>
#include <iomanip>

std::string jS(const std::string& s)
{
    std::string o = "\"";

    for (char c : s)
    {
        if (c == '"')
            o += "\\\"";
        else if (c == '\\')
            o += "\\\\";
        else if (c == '\n')
            o += "\\n";
        else if (c == '\r')
            o += "\\r";
        else if (c == '\t')
            o += "\\t";
        else
            o += c;
    }

    return o + '"';
}

std::string jVec(const std::vector<float>& v)
{
    std::ostringstream ss;

    ss << '[';

    for (size_t i = 0; i < v.size(); i++)
    {
        if (i)
            ss << ',';

        ss << std::fixed
           << std::setprecision(4)
           << v[i];
    }

    ss << ']';

    return ss.str();
}

std::vector<float> parseVec(const std::string& s)
{
    std::vector<float> v;

    std::istringstream ss(s);

    std::string t;

    while (std::getline(ss, t, ','))
    {
        try
        {
            v.push_back(std::stof(t));
        }
        catch (...)
        {
        }
    }

    return v;
}

std::string extractStr(
    const std::string& body,
    const std::string& key)
{
    size_t p = body.find('"' + key + '"');

    if (p == std::string::npos)
        return "";

    p = body.find(':', p) + 1;

    while (p < body.size() &&
          (body[p] == ' ' || body[p] == '\t'))
        p++;

    if (p >= body.size() || body[p] != '"')
        return "";

    p++;

    std::string result;

    while (p < body.size())
    {
        if (body[p] == '"')
            break;

        if (body[p] == '\\' && p + 1 < body.size())
        {
            p++;

            switch (body[p])
            {
                case '"':  result += '"'; break;
                case '\\': result += '\\'; break;
                case 'n':  result += '\n'; break;
                case 'r':  result += '\r'; break;
                case 't':  result += '\t'; break;
                default:   result += body[p];
            }
        }
        else
        {
            result += body[p];
        }

        p++;
    }

    return result;
}

int extractInt(
    const std::string& body,
    const std::string& key,
    int def)
{
    size_t p = body.find('"' + key + '"');

    if (p == std::string::npos)
        return def;

    p = body.find(':', p) + 1;

    while (p < body.size() &&
          (body[p] == ' ' || body[p] == '\t'))
        p++;

    try
    {
        return std::stoi(body.substr(p));
    }
    catch (...)
    {
        return def;
    }
}

bool parseBody(
    const std::string& body,
    std::string& metadata,
    std::string& category,
    std::vector<float>& embedding)
{
    metadata = extractStr(body, "metadata");
    category = extractStr(body, "category");

    auto extractArr =
    [&](const std::string& key)
    {
        size_t p = body.find('"' + key + '"');

        if (p == std::string::npos)
            return std::vector<float>{};

        p = body.find('[', p);

        if (p == std::string::npos)
            return std::vector<float>{};

        size_t e = body.find(']', p);

        if (e == std::string::npos)
            return std::vector<float>{};

        return parseVec(body.substr(p + 1, e - p - 1));
    };

    embedding = extractArr("embedding");

    return !metadata.empty() &&
           !embedding.empty();
}

void cors(httplib::Response& res)
{
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}