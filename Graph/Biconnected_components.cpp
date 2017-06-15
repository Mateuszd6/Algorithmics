/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: // TODO:  Time complexity: O(E+V). Space complexity: O(E+V).
Notes:
       * // TODO: 
===============================================================================*/

#include <iostream>
#include <vector>
#include "Graph.cpp"
#include "..\Utility.cpp"

// Assing pre order and low values for every vertex in a connected component of a given graph.
template <class VertexInfo, class EdgeInfo>
void _dfsSetLowFunction (Graph<VertexInfo, EdgeInfo> &graph, int current, int from, int &time, int* pre_ord, int* low)
{
    pre_ord[current] = time++;

    low[current] = pre_ord[current];
    for (int i = 0; i < graph[current].edges.size(); ++i)
    {
        if (graph[current].edges[i].to == from)
            continue;
        if (pre_ord[graph[current].edges[i].to] != 0)
        {
            low[current] = utility::Min(low[current], pre_ord[graph[current].edges[i].to]);
            continue;
        }
        _dfsSetLowFunction (graph, graph[current].edges[i].to, current, time, pre_ord, low);
        low[current] = utility::Min(low[current], low[graph[current].edges[i].to]);
    }
}

// Get all articulation points from the given graph.
template <class VertexInfo, class EdgeInfo>
std::vector <int> *ArticulationPoints(Graph<VertexInfo, EdgeInfo> &graph)
{
    // The result vertor to store id's of articulation points of the graph.
    std::vector<int> *result = new std::vector<int>(0);
    
    // True if node is a root of the DFS tree.
    bool *is_root = new bool[graph.Size()]();

    // Array to store preorder numbrer. 
    // pre_order == 0 means node has not been visited.
    int *pre_order = new int[graph.Size()]();
    
    // Time to determine when given node was reached by the algorithm.
    int time = 1;

    // Array to store low number.
    int *low = new int[graph.Size()];
    
    // Assing low function to every vertex in the graph.
    for (int i = 0; i < graph.Size(); ++i)
    {
        // If vertex is not explored yet...
        if (pre_order[i] == 0)
        {
            // Make it a root of the DFS subtree and start DFS here 
            // to assing low function in this connected component.
            is_root[i] = true;
            _dfsSetLowFunction(graph, i, -1, time, pre_order, low);
            
            // If there are two childs with different low values 
            // this node is an articulation point. 
            for (int j = 1; j < graph[0].edges.size(); ++j)
                if (low[graph[i].edges[j].to] != low[graph[i].edges[j-1].to])
                {
                    result->push_back(i);
                    break;
                }
        }
    }

    // Find the rest of articulation points:
    for (int i = 0; i < graph.Size(); ++i)
    {
        // DFS roots were checked before and following condition doesn't work here.
        if (is_root[i])
            continue;

        // For every edge that comes from current vertex..
        for (int j = 0; j < graph[i].edges.size(); ++j)

            // Every edge will be checked once, in the same way it was explored in the DFS algorithm.
            if (pre_order[graph[i].edges[j].from] < pre_order[graph[i].edges[j].to])
            {
                // This is the condition for articulation point. It must be true for at least one child.    
                if (low[graph[i].edges[j].to] >= pre_order[graph[i].edges[j].from])
                {
                    result->push_back(i);
                    break;
                }
            }
    }
    
    // Delete allocated data structures.
    delete[] is_root;
    delete[] pre_order;
    delete[] low;
    
    // Return the result.
    return result;
}

//*
//============== USAGE EXAMPLE ===================

//Empty struct as edges and vertices need no more additional info.
struct Empty { Empty(){} } ;

int main()
{
    Graph<Empty, Empty> g (18, undirected);
    g.AddEdge(13, 12, Empty());
    g.AddEdge(0, 1, Empty());
    g.AddEdge(1, 2, Empty());
    g.AddEdge(1, 13, Empty());
    g.AddEdge(2, 3, Empty());
    g.AddEdge(2, 7, Empty());
    g.AddEdge(3, 4, Empty());
    g.AddEdge(3, 5, Empty());
    g.AddEdge(4, 5, Empty());
    g.AddEdge(5, 6, Empty());
    g.AddEdge(7, 8, Empty());
    g.AddEdge(8, 9, Empty());
    g.AddEdge(10, 8, Empty());
    g.AddEdge(10, 11, Empty());
    g.AddEdge(12, 11, Empty());
    g.AddEdge(10, 12, Empty());
    g.AddEdge(13, 10, Empty());
    g.AddEdge(14, 0, Empty());
    g.AddEdge(15, 16, Empty());
    g.AddEdge(17, 15, Empty());
    
    std::vector <int> *tmp = ArticulationPoints(g);
    for (auto i : (* tmp))
        std::cout << i << "\n";

    return 0;
}
//*/