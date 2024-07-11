#pragma once

#include <vector>
#include <boost/filesystem.hpp>

#include "graph.h"

struct DependenciesTree
{
    struct Item
    {
        std::string name;
        boost::filesystem::path path;
    };

    struct Hash
    {
        size_t operator()(const Item &item) const
        {
            std::hash<std::string> hasher;
            return hasher(item.name) ^ hasher(item.path.string());
        }
    };

    struct Equal
    {
        bool operator()(const Item& lhs, const Item& rhs) const
        {
            return lhs.name == rhs.name && lhs.path == rhs.path;
        }
    };

    using Graph = Graph<Item, Hash, Equal>;
    using DfsCallback = std::function<void(const DependenciesTree::Item&, size_t)>;

    void depthFirstSearch(const DfsCallback &func) const;

    Graph graph;
    std::vector<Graph::Id> roots;
};
