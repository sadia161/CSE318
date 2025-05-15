#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

#include "graph.hpp"
#include "Constructive.hpp"
#include "LocalSearch.hpp"
#include "grasp.cpp"

Graph loadGraphFromFile(const string &filePath, const string &name)
{
    ifstream in(filePath);
    Graph graph;
    int n, m;
    in >> n >> m;
    graph.setName(name);
    graph.setSize(n, m);

    for (int i = 0; i < m; ++i)
    {
        int u, v;
        long long w;
        in >> u >> v >> w;
        graph.addEdge(u, v, w);
    }

    return graph;
}

int main()
{
    int graphNum;
    string algo;

    cout << "Enter graph number (e.g., 12 for g12.txt): ";
    cin >> graphNum;
    cout << "Enter algorithm (randomized / greedy / semi-greedy / local / grasp): ";
    cin >> algo;

    // Path to the graph file
    stringstream filename;
    filename << "g" << graphNum << ".txt";
    string path = "E:/3-1/CSE318/offline2/graph_GRASP/graph_GRASP/set1/" + filename.str();
    string graphName = "G" + to_string(graphNum);

    if (!fs::exists(path))
    {
        cout << "Graph file not found: " << path << endl;
        return 1;
    }

    Graph graph = loadGraphFromFile(path, graphName);

    if (algo == "randomized")
    {
        RandomizedMaxCut randAlgo(graph, 30);
        randAlgo.run();
        cout << "Randomized Cut (avg of 30): " << randAlgo.getAverageMaxCut() << endl;
    }
    else if (algo == "greedy")
    {
        GreedyMaxCut greedyAlgo(graph);
        greedyAlgo.run();
        cout << "Greedy Cut: " << greedyAlgo.getCutValue() << endl;
    }
    else if (algo == "semi-greedy")
    {
        SemiGreedyMaxCut semiGreedy(graph, 0.9);
        semiGreedy.run();
        cout << "Semi-Greedy Cut: " << semiGreedy.getCutValue() << endl;
    }
    else if (algo == "local")
    {
        SemiGreedyMaxCut semiGreedy(graph, 0.9);
        semiGreedy.run();
        vector<int> assign = semiGreedy.getAssignment();

        LocalSearchMaxCut local(graph);
        local.run(assign);
        cout << "Local Search Final Cut: " << local.getCutValue() << endl;
        cout << "Iterations: " << local.getIterations() << endl;
    }
    else if (algo == "grasp")
    {
        GraspMaxCut grasp(graph, 100, 0.9);
        grasp.run();
        cout << "GRASP Best Cut: " << grasp.getBestCutValue() << endl;
    }
    else
    {
        cout << "Unknown algorithm. Please use: randomized / greedy / semi-greedy / local / grasp" << endl;
    }

    return 0;
}
