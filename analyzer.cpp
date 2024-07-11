#include "analyzer.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

#include "parser.h"

using namespace std;

namespace fs = boost::filesystem;

namespace {
	fs::path resolve(const fs::path& path, const fs::path& base)
	{
		try {
			auto p = fs::canonical(path, base);
			return fs::is_regular_file(p) ? p : fs::path();
		} catch (const fs::filesystem_error &) {
			return {};
		}
	}

	fs::path resolve(const fs::path& path, const vector<fs::path>& base)
	{
		for (const auto &b : base) {
			auto p = resolve(path, b);
			if (!p.empty()) {
				return p;
			}
		}

		return {};
	}

	bool isValidFilename(const fs::path& path)
	{
		const auto ext = boost::algorithm::to_lower_copy(path.extension().string());
		return ext == ".h" || ext == ".cpp";
	}
}

Analyzer::Analyzer(const boost::filesystem::path& path) :
	m_src(path),
	m_parser(new Parser)
{

}

Analyzer::~Analyzer()
{

}

void Analyzer::addIncludePath(const boost::filesystem::path& path)
{
	m_includePaths.push_back(path);
}

DependenciesTree Analyzer::run() const
{
	DependenciesTree tree;

	for (const auto& entry : fs::recursive_directory_iterator(m_src)) {
		if (!fs::is_regular_file(entry)) {
			continue;
		}

		const auto path = fs::canonical(entry.path());

		if (!isValidFilename(path)) {
			continue;
		}

		DependenciesTree::Item item;
		item.name = path.filename().string();
		item.path = path;

		tree.roots.push_back(process(tree.graph, tree.graph.nullId(), item));
	}

	return tree;
}

DependenciesTree::Graph::Id Analyzer::process(DependenciesTree::Graph& graph, DependenciesTree::Graph::Id parent,
											  const DependenciesTree::Item& item) const
{
	bool alreadyVisited = false;
	const auto vid = graph.addVertex(item, &alreadyVisited);

	if (parent != graph.nullId()) {
		graph.addEdge(parent, vid);
	}

	if (alreadyVisited) {
		return vid;
	}

	const auto& path = item.path;
	if (path.empty()) {
		return vid;
	}

	const auto includes = m_parser->run(path);

	const auto parentPath = path.parent_path();

	for (const auto& include : includes) {
		DependenciesTree::Item child;
		child.name = include.path.filename().string();

		if (include.scope == Parser::Item::Scope::Local) {
			child.path = resolve(include.path, parentPath);
		} else {
			child.path = resolve(include.path, m_includePaths);
		}

		process(graph, vid, child);
	}

	return vid;
}


