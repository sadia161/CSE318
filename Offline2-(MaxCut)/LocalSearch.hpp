#ifndef LOCALSEARCH_HPP
#define LOCALSEARCH_HPP

#include <vector>
#include <climits>
#include <iostream>
using namespace std;

#include "graph.hpp"
#include "Constructive.hpp"

class LocalSearchMaxCut
{
private:
    Graph graph;
    vector<int> assignment;
    long long cutValue;
    int iterations;

public:
    LocalSearchMaxCut(Graph &_graph)
    {
        graph = _graph;
        cutValue = 0;
        iterations = 0;
    }

    // void run(vector<int> initialAssignment)
    // {
    //     // cout << "\n=== Local Search (Delta-based) ===\n";

    //     assignment = initialAssignment;
    //     int n = graph.getVertexCount();

    //     cutValue = graph.getMaxCut(assignment);

    //     bool improved = true;

    //     while (improved)
    //     {
    //         improved = false;

    //         for (int v = 1; v <= n; ++v)
    //         {
    //             int flipped = 1 - assignment[v]; // flip the vertex assignment

    //             // simulate flip
    //             assignment[v] = flipped;

    //             long long newCut = graph.getUpdatedMaxCut(v, cutValue, assignment);

    //             if (newCut > cutValue)
    //             {
    //                 cutValue = newCut;
    //                 improved = true;
    //                 iterations++;
    //                 break;
    //             }
    //             else
    //             {
    //                 // revert flip
    //                 assignment[v] = 1 - flipped;
    //             }
    //         }
    //     }

    //     // cout << "  Final Cut Weight after Local Search: " << cutValue << endl;
    //     // cout << "  Iterations: " << iterations << endl;
    // }

    void run(vector<int> initialAssignment)
    {
        assignment = initialAssignment;
        int n = graph.getVertexCount();
        cutValue = graph.getMaxCut(assignment);
        bool improved = true;

        while (improved)
        {
            improved = false;
            int bestVertex = -1;
            long long bestDelta = 0;

            for (int v = 1; v <= n; ++v)
            {
                int flipped = 1 - assignment[v]; // flip
                assignment[v] = flipped;

                long long newCut = graph.getUpdatedMaxCut(v, cutValue, assignment);
                long long delta = newCut - cutValue;

                if (delta > bestDelta)
                {
                    bestDelta = delta;
                    bestVertex = v;
                }

                assignment[v] = 1 - flipped; // revert the flip
            }

            if (bestVertex != -1)
            {
                assignment[bestVertex] = 1 - assignment[bestVertex];
                cutValue += bestDelta;
                improved = true;
                iterations++;
            }
        }
    }

    vector<int> getAssignment()
    {
        return assignment;
    }

    long long getCutValue()
    {
        return cutValue;
    }

    int getIterations()
    {
        return iterations;
    }
};

#endif
