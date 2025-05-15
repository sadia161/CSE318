#include "Constructive.hpp"
#include "LocalSearch.hpp"
#include "graph.hpp"

#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include <climits>

using namespace std;

class GraspMaxCut
{
private:
    Graph graph;
    vector<int> bestAssignment;
    long long bestCutValue;
    int bestIterations;
    int maxIterations;
    double alpha;
    mt19937 &rng;

public:
    // GraspMaxCut(Graph &_graph, int _maxIterations = 50, double _alpha = 0.9)
    // {
    //     graph = _graph;
    //     maxIterations = _maxIterations;
    //     alpha = _alpha;
    //     bestCutValue = LLONG_MIN;
    //     bestIterations = 0;
    // }

    GraspMaxCut(Graph &_graph, mt19937 &_rng, int _maxIterations = 50, double _alpha = 0.9)
        : graph(_graph), rng(_rng), maxIterations(_maxIterations), alpha(_alpha)
    {
        bestCutValue = LLONG_MIN;
        bestIterations = 0;
    }

    void run()
    {
        // cout << "\n=== GRASP (Semi-Greedy + Local Search) ===\n";

        for (int i = 1; i <= maxIterations; ++i)
        {
            SemiGreedyMaxCut semiGreedy(graph, alpha, rng);
            semiGreedy.run();

            vector<int> initialAssignment = semiGreedy.getAssignment();

            LocalSearchMaxCut localSearch(graph);
            localSearch.run(initialAssignment);

            long long currentCut = localSearch.getCutValue();

            if (i == 1 || currentCut > bestCutValue)
            {
                bestCutValue = currentCut;
                bestAssignment = localSearch.getAssignment();
                bestIterations = localSearch.getIterations();
            }
        }

        // cout << "  Best Cut Found After GRASP: " << bestCutValue << endl;
        // cout << "  Total Iterations (best run): " << bestIterations << endl;
    }

    vector<int> getBestAssignment()
    {
        return bestAssignment;
    }

    long long getBestCutValue()
    {
        return bestCutValue;
    }

    int getBestIterations()
    {
        return bestIterations;
    }
};

// int main()
// {
//     // taking input from a file using freopen
//     freopen("g1.txt", "r", stdin);
//     freopen("out.txt", "w", stdout);
//     Graph g;
//     g.setName("G1");
//     int n, m;
//     cin >> n >> m;

//     g.setSize(n, m);

//     for (int i = 0; i < m; i++)
//     {
//         long long u, v;
//         long long w;
//         // cout << "adding edge " << i + 1 << endl;
//         cin >> u >> v >> w;
//         g.addEdge(u, v, w);
//     }

//     // RandomizedMaxCut rmc(g, 1000);
//     // double result = rmc.run();
//     // cout << "Average cut value: " << result << endl;

//     // GreedyMaxCut gmc(g);
//     // gmc.run();
//     // vector<int> assignment = gmc.getAssignment();
//     // long long result = gmc.getCutValue();
//     // cout << "Greedy cut value: " << result << endl;

//     SemiGreedyMaxCut sg(g, 0.8); // alpha = 0.3 for some randomness
//     sg.run();
//     vector<int> initialAssignment = sg.getAssignment();
//     long long initialCutValue = sg.getCutValue();
//     cout << "Initial Cut Value: " << initialCutValue << endl;

//     LocalSearchMaxCut lsmc(g);
//     lsmc.run(initialAssignment);
//     vector<int> finalAssignment = lsmc.getAssignment();
//     long long finalCutValue = lsmc.getCutValue();
//     cout << "Final Cut Value after Local Search: " << finalCutValue << endl;

//     GraspMaxCut grasp(g, 50, 0.8); // maxIterations = 50, alpha = 0.9
//     grasp.run();
//     vector<int> bestAssignment = grasp.getBestAssignment();
//     long long bestCutValue = grasp.getBestCutValue();
//     int bestIterations = grasp.getBestIterations();
//     cout << "Best Cut Value after GRASP: " << bestCutValue << endl;
//     cout << "Total Iterations (best run): " << bestIterations << endl;

//     return 0;
// }
