#pragma once

#include <vector>
#include <memory>
#include <boost/filesystem.hpp>

#include "dependenciestree.h"

class Parser;

class Analyzer
{
public:
    explicit Analyzer(const boost::filesystem::path& path);
    ~Analyzer();

    void addIncludePath(const boost::filesystem::path& path);

    DependenciesTree run() const;
private:
    DependenciesTree::Graph::Id Analyzer::process(DependenciesTree::Graph& graph, DependenciesTree::Graph::Id parent,
                                                  const DependenciesTree::Item& item) const;

    boost::filesystem::path m_src;
    std::vector<boost::filesystem::path> m_includePaths;

    std::unique_ptr<Parser> m_parser;
};

