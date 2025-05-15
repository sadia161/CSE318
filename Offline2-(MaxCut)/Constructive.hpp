#ifndef CONSTRUCTIVE_HPP
#define CONSTRUCTIVE_HPP

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <tuple>
#include <map>
#include <utility>
#include <climits>
#include <cstdlib>
#include <cmath>

#include "graph.hpp"

using namespace std;

class RandomizedMaxCut
{
private:
    Graph graph;
    int maxIterations;
    mt19937 &rng;
    double averageCutValue;

public:
    // RandomizedMaxCut(Graph &_graph, mt19937 &_rng )
    // {
    //     graph = _graph;
    //     // rng.seed(time(0));
    //     rng = _rng;
    // }

    RandomizedMaxCut(Graph &_graph, mt19937 &_rng, int _maxIterations = 50)
        : graph(_graph), rng(_rng), maxIterations(_maxIterations)
    {
        // averageCutValue = 0.0;
    }

    double run()
    {
        long long totalCutWeight = 0;
        int n = graph.getVertexCount();

        for (int i = 0; i < maxIterations; i++)
        {
            vector<int> assignment(n);
            for (int v = 0; v < n; v++)
            {
                assignment[v] = rng() % 2; // Random 0 or 1
            }

            long long cutValue = graph.getMaxCut(assignment);

            totalCutWeight += cutValue;
        }

        // average
        averageCutValue = (double)totalCutWeight / maxIterations;
        cout << "  Average Cut Weight: " << averageCutValue << endl;
        return (double)totalCutWeight / maxIterations;
    }

    double getAverageMaxCut()
    {
        return averageCutValue;
    }
};

class GreedyMaxCut
{
private:
    Graph graph;
    vector<int> assignment;
    long long cutValue;

public:
    GreedyMaxCut(Graph &_graph)
    {
        graph = _graph;
        assignment.resize(graph.getVertexCount() + 1, -1);
        cutValue = 0;
    }

    void run()
    {
        // cout << "\n=== Greedy Heuristic ===\n";

        int n = graph.getVertexCount();
        auto edges = graph.getEdges();
        auto adjList = graph.getAdjList();

        // Find maximum weight
        Edge maxEdge;
        long long maxWeight = LLONG_MIN;

        for (auto &e : edges)
        {
            if (e.w > maxWeight)
            {
                maxWeight = e.w;
                maxEdge = e;
            }
        }

        if (maxWeight == LLONG_MIN)
        {
            cout << "  No edges found. Cut weight = 0\n";
            cutValue = 0;
            return;
        }

        assignment.assign(n + 1, -1);

        assignment[maxEdge.u] = 0; // X
        assignment[maxEdge.v] = 1; // Y

        for (int z = 1; z <= n; ++z)
        {
            if (assignment[z] != -1)
                continue;

            long long wX = 0, wY = 0;

            for (auto &neighbor : adjList[z])
            {
                int u = neighbor.first;
                long long w = neighbor.second;

                if (assignment[u] == 0)
                    wY += w; // putting z in Y cuts the edge
                else if (assignment[u] == 1)
                    wX += w; // putting z in X cuts the edge
            }

            if (wX > wY)
                assignment[z] = 0;
            else
                assignment[z] = 1;
        }

        cutValue = graph.getMaxCut(assignment);
        // cout << "  Cut Weight: " << cutValue << endl;
    }

    vector<int> getAssignment()
    {
        return assignment;
    }

    long long getCutValue()
    {
        return cutValue;
    }
};

class SemiGreedyMaxCut
{
private:
    Graph graph;
    vector<int> assignment;
    long long cutValue;
    double alpha;
    mt19937 &rng;

public:
    // SemiGreedyMaxCut(Graph &_graph, double _alpha = 0.9)
    // {
    //     graph = _graph;
    //     alpha = _alpha;
    //     assignment.resize(graph.getVertexCount() + 1, -1);
    //     cutValue = 0;
    //     srand(time(0));
    // }

    SemiGreedyMaxCut(Graph &_graph, double _alpha, mt19937 &_rng)
        : graph(_graph), alpha(_alpha), rng(_rng)
    {
        assignment.resize(graph.getVertexCount() + 1, -1);
        cutValue = 0;
    }

    void run()
    {
        // cout << "\n=== Semi-Greedy Heuristic (Value-based RCL) ===\n";

        int n = graph.getVertexCount();
        auto edges = graph.getEdges();
        auto adjList = graph.getAdjList();

        assignment.assign(n + 1, -1);

        Edge maxEdge;
        long long maxWeight = LLONG_MIN;
        for (auto &e : edges)
        {
            if (e.w > maxWeight)
            {
                maxWeight = e.w;
                maxEdge = e;
            }
        }

        if (maxWeight == LLONG_MIN)
        {
            cout << "  No edges found. Cut weight = 0\n";
            cutValue = 0;
            return;
        }

        assignment[maxEdge.u] = 0;
        assignment[maxEdge.v] = 1;

        int assignedCount = 2;

        while (assignedCount < n)
        {
            vector<int> candidates;
            vector<long long> sigmaX(n + 1, 0);
            vector<long long> sigmaY(n + 1, 0);
            vector<long long> greedyScore(n + 1, 0);

            long long wMin = LLONG_MAX;
            long long wMax = LLONG_MIN;

            for (int v = 1; v <= n; v++)
            {
                if (assignment[v] != -1)
                    continue;

                for (auto &nbr : adjList[v])
                {
                    int u = nbr.first;
                    long long w = nbr.second;

                    if (assignment[u] == 0)
                        sigmaX[v] += w;
                    else if (assignment[u] == 1)
                        sigmaY[v] += w;
                }

                greedyScore[v] = max(sigmaX[v], sigmaY[v]);
                wMin = min(wMin, greedyScore[v]);
                wMax = max(wMax, greedyScore[v]);
            }

            if (wMin > wMax)
                break; // edge case: no unassigned neighbors

            double cutoff = wMin + alpha * (wMax - wMin);
            for (int v = 1; v <= n; v++)
            {
                if (assignment[v] == -1 && greedyScore[v] >= cutoff)
                {
                    candidates.push_back(v);
                }
            }

            if (candidates.empty())
            {
                cout << "  No candidates found, exiting loop early.\n";
                break;
            }

            // int randIndex = rand() % candidates.size();
            int randIndex = rng() % candidates.size();

            int selected = candidates[randIndex];

            if (sigmaX[selected] > sigmaY[selected])
                assignment[selected] = 1;
            else
                assignment[selected] = 0;

            assignedCount++;
        }

        cutValue = graph.getMaxCut(assignment);
        // cout << "  Final Cut Weight: " << cutValue << endl;
    }

    vector<int> getAssignment()
    {
        return assignment;
    }

    long long getCutValue()
    {
        return cutValue;
    }
};

#endif // CONSTRUCTIVE_HPP