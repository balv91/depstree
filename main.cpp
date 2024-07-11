#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>

#include "analyzer.h"
#include "dependenciestree.h"

using namespace std;
using namespace boost;

int main(int argc, char *argv[])
{
    namespace po = program_options;

    const auto srcOps = "src";
    const auto includePathOps = "includepath";
    const auto includePathFullOps = "includepath,I";

    po::options_description desc("Options");
    desc.add_options()
        (srcOps, po::value<filesystem::path>(), "src path")
        (includePathFullOps, po::value<vector<filesystem::path>>(), "include path");

    po::positional_options_description p;
    p.add(srcOps, -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

    if (!vm.count(srcOps)) {
        cout << desc << endl;
        return 1;
    }

    Analyzer analyzer(vm[srcOps].as<filesystem::path>());

    if (vm.count(includePathOps)) {
        for (const auto &path : vm[includePathOps].as<vector<filesystem::path>>()) {
            analyzer.addIncludePath(path);
        }
    }

    const auto tree = analyzer.run();

    tree.depthFirstSearch([] (const DependenciesTree::Item &item, size_t level) {
        cout << setfill('.') << setw(level + item.name.size()) << item.name;

        if (item.path.empty()) {
            cout << " (!)";
        }

        cout << '\n';
    });

    cout << '\n';

    {
        unordered_map<string, size_t> frequencyMap;

        tree.depthFirstSearch([&frequencyMap](const DependenciesTree::Item& item, size_t level) {
            auto& f = frequencyMap[item.name];
            if (level) {
                ++f;
            }
        });

        vector<pair<string, size_t>> frequencyList(frequencyMap.cbegin(), frequencyMap.cend());
        sort(std::begin(frequencyList), std::end(frequencyList), [](const auto& a, const auto& b) {
            if (a.second == b.second) {
                return a.first < b.first;
            }

            return a.second > b.second;
        });

        for (const auto& p : frequencyList) {
            cout << p.first << ' ' << p.second << '\n';
        }
    }

    return 0;
}
