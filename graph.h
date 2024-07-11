#pragma once

#include <vector>
#include <unordered_map>
#include <utility>

template <typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>>
class Graph
{
public:
    using Id = size_t;
    static const Id NullId = -1;

    Id addVertex(const T& vertex, bool *exists = nullptr)
    {
        auto it = m_verticesMap.find(vertex);
        if (it != m_verticesMap.end()) {
            if (exists) {
                *exists = true;
            }

            return it->second;
        }

        m_vertices.push_back(vertex);
        const auto id = m_vertices.size() - 1;
        m_verticesMap.insert(make_pair(vertex, id));

        if (exists) {
            *exists = false;
        }

        return id;
    }

    const T& vertex(Id id) const
    {
        return m_vertices.at(id);
    }

    Id vertexId(const T& vertex) const
    {
        auto it = m_verticesMap.find(vertex);
        return it != m_verticesMap.end() ? it->second : NullId;
    }

    void addEdge(Id from, Id to)
    {
        if (from == NullId || to == NullId) {
            return;
        }

        auto range = m_edges.equal_range(from);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == to) {
                return;
            }
        }

        m_edges.insert(make_pair(from, to));
    }

    std::vector<Id> edges(Id from) const
    {
        std::vector<Id> result;

        auto range = m_edges.equal_range(from);
        
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back(it->second);
        }

        return result;
    }

    size_t vertices() const
    {
        return m_vertices.size();
    }

    Id nullId() const
    {
        return NullId;
    }
private:
    std::unordered_multimap<Id, Id> m_edges;
    std::unordered_map<T, Id, Hash, Equal> m_verticesMap;
    std::vector<T> m_vertices;
};
