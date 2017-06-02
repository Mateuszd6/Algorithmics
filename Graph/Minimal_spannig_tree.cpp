/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Kruskal's algorithm for constructing minimal spannig tree (MST). Time complexity: O(E*log(E)). Space complexity: O(E+V).
Notes:
       * This algorithm sorts all edges and picks form them creating the minimal spanning tree from the given graph.
       * It used disjoint set data structure (also know as find-union).
       * Almost in every case it works faster than Prim's algorithm, despite same time complexity, however the difference is little. 
       * This algorithm works only with undirected graphs, as the minimal spanning tree problem refers only to them.
===============================================================================*/

#include <iostream>
#include <vector>
#include <algorithm>
#include "Graph.cpp"
#include "..\Data_structures\Disjoint_set.cpp"

// Find the minimal spanning tree of the graph using Kruskal's algorithm.
// Given a graph return the weight of the spanning tree. It is also possible to pass
// pointer to the empty graph with same size, to get the graph representing the spanning tree.
// If no tree needs to be generated outputTree should be passed NULL. 
template <class VertexInfo, class EdgeInfo>
int MinimalSpanningTree (Graph<VertexInfo, EdgeInfo> &graph, void* outputTree)
{
	static_assert(std::is_base_of<DistanceEdge, EdgeInfo>::value, "EdgeInfo must inherit from DistanceEdge");
    bool fillOutputTree = (outputTree != NULL);
    Graph<VertexInfo, EdgeInfo> * output = (Graph<VertexInfo, EdgeInfo> *)outputTree;
    std::vector < Graph<VertexInfo, EdgeInfo> :: Edge > edges;
    DisjointSetArray *vertices = new DisjointSetArray(graph.Size());
    int result = 0;

    for (int i = 0; i < graph.Size(); ++i)
        for (int j = 0; j < graph[i].edges.size(); ++j)
            edges.push_back(graph[i].edges[j]);
    std::sort(edges.begin(), edges.end());
    for (int i = 0; i < edges.size(); ++i)
    {
        if (vertices->Find(edges[i].to) != vertices->Find(edges[i].from))
        {
            if (fillOutputTree)
                output->AddEdge(edges[i].from, edges[i].to, (EdgeInfo)edges[i]);
            result += edges[i].weight;
            vertices->Merge(edges[i].to, edges[i].from);
        }
    }

    delete vertices;

    return result;
}

// Get the weight of the minimal spanning tree of the given graph, when no output tree mu be generated.
// This is the same as passing NULL to the outputTree parameter to the function above.
template <class VertexInfo, class EdgeInfo>
int MinimalSpanningTree (Graph<VertexInfo, EdgeInfo> &graph)
{
    return MinimalSpanningTree(graph, NULL);
}

//*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices need no more additional info.
struct Empty { Empty() {} };

int main ()
{
	Graph<Empty, DistanceEdge> G(9, undirected);
	G.AddEdge(1, 2, DistanceEdge(3));
	G.AddEdge(3, 1, DistanceEdge(1));
	G.AddEdge(3, 2, DistanceEdge(1));
	G.AddEdge(5, 3, DistanceEdge(1));
	G.AddEdge(3, 6, DistanceEdge(7));
	G.AddEdge(0, 6, DistanceEdge(2));
	G.AddEdge(5, 0, DistanceEdge(4));
	G.AddEdge(0, 4, DistanceEdge(1));
	G.AddEdge(5, 4, DistanceEdge(2));
	G.AddEdge(7, 8, DistanceEdge(4));
    Graph<Empty, DistanceEdge> * outputSpanningTree = new Graph<Empty, DistanceEdge>(9, undirected); 
    std:: cout << "Minimal spanning tree weight: " << MinimalSpanningTree(G, outputSpanningTree) << ".\nMinimal spanning tree: ";
    std:: cout << (* outputSpanningTree);
    //Output:
    //  Minimal spanning tree weight: 12.
    //  Minimal spanning tree: Graph with 9 vertices, undirected:
    //  0:  4 6 
    //  1:  3 
    //  2:  3 
    //  3:  1 2 5 
    //  4:  0 5 
    //  5:  3 4 
    //  6:  0 
    //  7:  8 
    //  8:  7 

    return 0;
}
//*/