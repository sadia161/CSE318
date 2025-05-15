#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <random>
#include <ctime>

#include "Constructive.hpp"
#include "LocalSearch.hpp"
#include "graph.hpp"
#include "grasp.cpp"

using namespace std;
namespace fs = std::filesystem;

vector<int> knownBestSolutions = {-1,
                                  12078, 12084, 12077, -1, -1, -1, -1, -1, -1, -1,
                                  627, 621, 645, 3187, 3169, 3172, -1, -1, -1, -1,
                                  -1, 14123, 14129, 14131, -1, -1, -1, -1, -1, -1,
                                  -1, 1560, 1537, 1541, 8000, 7996, 8009, -1, -1, -1,
                                  -1, -1, 7027, 7022, 7020, -1, -1, 6000, 6000, 5988,
                                  -1, -1, -1, -1};

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
    ofstream csv("2105161.csv");
    if (!csv.is_open())
    {
        cerr << "Failed to open output CSV file." << endl;
        return 1;
    }

    mt19937 rng(time(0));

    double alpha = 0.9;

    // csv << "Name,|V| or n,|E| or m,"
    //     << "Simple Randomized, Simple Greedy,"
    //     << "\"Semi-Greedy-1\n(alpha=" << alpha << ")\","
    //     << "\"Simple Local\nIterations\",\"Simple Local\nAverage Value\","
    //     << "\"GRASP-1\nIterations\",\"GRASP-1\nBest Value\","
    //     << "\"Known Best\nSolution\"" << endl;

    csv << " Problem details,,, Constructive Algorithm,,, Local Search,, GRASP algorithm,, Known best solution or upper bound\n";
    csv << " Name, |V| or n, |E| or m, Simple Randomized, Greedy-1, Semi-Greedy-1 (alpha=" << alpha << "),  No. of Iterations, Best Value, No. of Iterations, Best Value\n";

    // string graphFolder = "CSE318/offline2/graph_GRASP/graph_GRASP/set1/";

    string graphFolder = "E:\\3-1\\CSE318\\offline2\\graph_GRASP\\graph_GRASP\\set1\\";

    for (int i = 1; i <= 54; ++i)
    {
        stringstream filename;
        filename << "g" << i << ".txt";

        string path = graphFolder + filename.str();
        string graphName = "G" + to_string(i);

        if (!fs::exists(path))
            continue;

        if (!fs::exists(path))
        {
            cout << "Skipping: " << path << " (not found)" << endl;
            continue;
        }

        Graph graph = loadGraphFromFile(path, graphName);

        int n = graph.getVertexCount();
        int m = graph.getEdgeCount();

        // Randomized
        RandomizedMaxCut randomized(graph, rng);
        randomized.run();
        long long randomizedCut = randomized.getAverageMaxCut();

        // Greedy
        GreedyMaxCut greedy(graph);
        greedy.run();
        long long greedyCut = greedy.getCutValue();

        // Semi-Greedy

        SemiGreedyMaxCut semiGreedy(graph, alpha, rng);
        semiGreedy.run();
        long long semiGreedyCut = semiGreedy.getCutValue();

        // Local Search on semi-greedy result

        // LocalSearchMaxCut localSearch(graph);
        // localSearch.run(semiGreedy.getAssignment());
        // long long localCut = localSearch.getCutValue();
        // int localIters = localSearch.getIterations();

        int localIters = 20;
        long long localCut = 0;
        for (int j = 0; j < localIters; ++j)
        {
            SemiGreedyMaxCut semiGreedy(graph, alpha, rng);
            semiGreedy.run();
            LocalSearchMaxCut localSearch(graph);
            localSearch.run(semiGreedy.getAssignment());
            localCut += localSearch.getCutValue();
        }
        localCut /= localIters;

        // GRASP (Semi-Greedy + Local)
        int graspIters = 100;
        GraspMaxCut grasp(graph, rng, graspIters, 0.9);
        grasp.run();
        long long graspBest = grasp.getBestCutValue();

        int knownBest = (i < knownBestSolutions.size()) ? knownBestSolutions[i] : -1;

        csv << graphName << "," << n << "," << m << ","
            << randomizedCut << ","                  // Simple Randomized
            << greedyCut << ","                      // Greedy-1
            << semiGreedyCut << ","                  // Semi-Greedy-1 + 2 empty columns for Greedy-2 and Semi-Greedy-2
            << localIters << "," << localCut << ","  // Local Search for Semi-Greedy-1
            << graspIters << "," << graspBest << "," // GRASP for Semi-Greedy-1
            << (knownBest == -1 ? "" : to_string(knownBest))
            << endl;
    }

    csv.close();
    cout << " Results saved to 2105161.csv" << endl;
    return 0;
}