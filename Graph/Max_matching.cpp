/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Turbomatching algorithm. Time complexity: O((V+E)*V). Space complexity: O(V).
Notes:
       * Finds the maximal matching of the bipartite graph.
       * This algorithm works much faster than it's complexity suggest. In most cases it is faster 
         than Hopcroft-Karp algorithm. 
===============================================================================*/

#include <iostream>
#include "Graph.cpp"

namespace detail
{
    // DFS procedure that tries to improve matching and returns if it succeeded.
    template <class VertexInfo, class EdgeInfo>
    bool DFSTurboMatching(Graph<VertexInfo, EdgeInfo> &graph, int current, bool *visited, int *matched)
    {
        // If vertex was visited this method will return false.
        if (!visited[current])
        {
            visited[current] = true;
            for (auto edge : graph[current].edges)
            {
                // If end of the edge is not matched or if it is, but can be improved,
                // (the matching was changed recursively) match ends of the current edge and return true.
                if (matched[edge.to] == -1 || DFSTurboMatching(graph, matched[edge.to], visited, matched))
                {
                    matched[edge.to] = current;
                    matched[current] = edge.to;
                    return true;
                }
            }
        }
        return false;
    }
}

// Finds the maximal matching of the bipartite graph.
// Returns -1 if given graph is not bipartite.
// vector of edges can be passed by pointer to store the edges included in the matching.
template <class VertexInfo, class EdgeInfo>
int TurboMatching(Graph<VertexInfo, EdgeInfo> &graph, 
    std::vector<typename Graph<VertexInfo, EdgeInfo>::Edge> *foo)
{
    // Vertex stores indexes of vertices in the first group of the graph. 
    std::vector <int> vert_in_first;
    
    // Graph must be Bipartite.
    if (!graph.IsBipartite(&vert_in_first))
    {
        std::cerr << "Cannot find the matching because graph is not bipartite.\n";
        return -1;
    }

    bool *visited = new bool[graph.Size()];
    int *matched = new int[graph.Size()];
    // -1 means vertex is unmatched by the algorithm yet.
    // At start nothing is matched.
    for (int i = 0; i < graph.Size(); ++i)
        matched[i] = -1;

    // Has number of matched edges increased?
    bool improved_matching = true;

    while (improved_matching)
    {
        // Clear the improved_matching and visited array.
        improved_matching = false;
        for (int i = 0; i < graph.Size(); ++i)
            visited[i] = false;

        // Search for unmatched vertex in the first set.
        // If every vertex of the set is matched, matching has reached its maximum size.
        for (int i : vert_in_first)
        {       
            if (matched[i] == -1)
            {
                // Try to improve matching.
                bool improved = detail::DFSTurboMatching(graph, i, visited, matched);
                if (improved)
                    improved_matching = true;
            }
        }   
    }

    // Get the number of matched edges from the matched array.
    int result = 0;
    for (int i : vert_in_first)
        if (matched[i])
            ++result;

    // Fill the edge vector if needed.
    if (foo != NULL)
    {
        for (int i : vert_in_first)
            for (auto edge : graph[i].edges)
                if (matched[edge.from] == edge.to)
                    foo->push_back(edge);
    }

    delete[] visited;
    delete[] matched;

    return result;
}
/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices need no more additional info.
struct Empty { Empty() {} };

int main()
{
    Graph<Empty, Empty> G(7, undirected);
    G.AddEdge(0, 3, Empty());
    G.AddEdge(2, 1, Empty());
    G.AddEdge(2, 3, Empty());
    G.AddEdge(2, 5, Empty());
    G.AddEdge(4, 3, Empty());
    G.AddEdge(4, 5, Empty());
    G.AddEdge(4, 6, Empty());
    std::vector<Graph<Empty,Empty>::Edge> foo;
    std::cout << G << "Max matching of this graph: " << TurboMatching(G, &foo) << "\n";
    std::cout << "Edges in the matching: ";
    for (auto i : foo)
        std::cout << i.from << "-" << i.to << " ";
    //Output:
    //  Graph with 7 vertices, undirected:
    //  0:  3
    //  1:  2
    //  2:  1 3 5
    //  3:  0 2 4
    //  4:  3 5 6
    //  5:  2 4
    //  6:  4
    //  Max matching of this graph: 3
    //  Edges in the matching: 0-3 2-1 4-5
    
    return 0;
}
*/
