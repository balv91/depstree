#include "dependenciestree.h"

#include <unordered_set>

using namespace std;

namespace {
    void depthFirstSearch(const DependenciesTree::Graph &graph, DependenciesTree::Graph::Id id,
                          unordered_set<DependenciesTree::Graph::Id> &visited, 
                          const DependenciesTree::DfsCallback &func, size_t level = 0)
    {
        if (visited.find(id) != visited.end()) {
            return;
        }

        func(graph.vertex(id), level);

        visited.insert(id);

        for (const auto& childId : graph.edges(id)) {
            depthFirstSearch(graph, childId, visited, func, level + 1);
        }
    }
}

void DependenciesTree::depthFirstSearch(const DfsCallback &func) const
{
    unordered_set<DependenciesTree::Graph::Id> visited;
    visited.reserve(graph.vertices());

    for (const auto& id : roots) {
        ::depthFirstSearch(graph, id, visited, func, 0);
        visited.clear();
    }
}