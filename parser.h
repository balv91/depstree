#pragma once

#include <vector>

#include <boost/filesystem.hpp>

class Tokenizer;

class Parser
{
public:
    struct Item
    {
        enum class Scope {
            Local,
            Global
        };

        Scope scope;
        boost::filesystem::path path;
    };

    using ItemList = std::vector<Item>;

    Parser();
    ~Parser();

    ItemList run(const boost::filesystem::path& path);
private:
    std::unique_ptr<Tokenizer> m_tokenizer;
};