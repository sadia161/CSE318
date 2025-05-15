#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <string>
using namespace std;

class Edge
{
public:
    int u, v;
    long long w;

    Edge(int _u = 0, int _v = 0, long long _w = 0)
    {
        u = _u;
        v = _v;
        w = _w;
    }

    bool operator<(const Edge &other) const
    {
        return w < other.w;
    }
    bool operator>(const Edge &other) const
    {
        return w > other.w;
    }
};

class Graph
{
private:
    int n, m;
    string name;
    vector<Edge> edges;
    vector<vector<pair<int, long long>>> adjList;

public:
    Graph() {}

    void setName(string _name)
    {
        name = _name;
    }

    void setSize(int _n, int _m)
    {
        n = _n;
        m = _m;
        adjList.assign(n + 1, vector<pair<int, long long>>());
    }

    void addEdge(int u, int v, long long w)
    {
        edges.push_back(Edge(u, v, w));
        adjList[u].push_back({v, w});
        adjList[v].push_back({u, w});
    }

    string getName()
    {
        return name;
    }

    int getVertexCount()
    {
        return n;
    }

    int getEdgeCount()
    {
        return m;
    }

    vector<Edge> getEdges()
    {
        return edges;
    }

    vector<vector<pair<int, long long>>> getAdjList()
    {
        return adjList;
    }

    long long getMaxCut(const vector<int> &assignment)
    {
        long long cutValue = 0;
        for (auto edge : edges)
        {
            if (assignment[edge.u] != assignment[edge.v])
            {
                cutValue += edge.w;
            }
        }
        return cutValue;
    }

    long long getUpdatedMaxCut(int changedIndex, long long previousCut, const vector<int> &assignment)
    // this function is used to update the cut value when a vertex's assignment changes
    // changedIndex is the index of the vertex whose assignment has changed
    // previousCut is the cut value before the change
    // assignment is the current assignment of all vertices
    // We need to check all edges connected to changedIndex and update the cut value accordingly

    {
        long long newCut = previousCut;
        // Iterate through all edges connected to changedIndex
        // and update the cut value based on the new assignment
        for (auto adj : adjList[changedIndex])
        {
            int neighbor = adj.first;
            long long weight = adj.second;

            if (assignment[changedIndex] == assignment[neighbor])
            {
                newCut -= weight;
            }
            else
            {
                newCut += weight;
            }
        }
        return newCut;
    }
};

#endif
