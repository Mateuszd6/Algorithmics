/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Kosaraju's algorithm for strongly connected components. Time complexity: O(E+V). Space complexity: O(E+V).
Notes:
       * Finds all strongly connected components in the graph. 
       * It uses Kosaraju's algorithm and standard graph reversing (check Graph class for details).
       * The algorithm works only with directed graph as strongly connected components problem refers only to them.
===============================================================================*/

#include <iostream>
#include <vector>
#include "Graph.cpp"

// Traverse the graph if depth-first way and push every fully explored vertex to the post_order vector.
template <class VertexInfo, class EdgeInfo>
void DFSNumeratePostOrder(int current, Graph<VertexInfo, EdgeInfo> &graph, 
    std::vector<int> *post_order, bool * visited)
{    
    visited[current] = true;

    for (int j = 0; j < graph[current].edges.size(); ++j)
        if (!visited[graph[current].edges[j].to])
            DFSNumeratePostOrder(graph[current].edges[j].to, graph, post_order, visited);

    // Put the vertex at the end of the post_order vector.
    post_order->push_back(current);
}

// Used to traverse reversed graph. Every vertex that is reachable from the source vertex will be 
// in one strongly connected component.
template <class VertexInfo, class EdgeInfo>
void DFSPutIntoSCC(int current, Graph<VertexInfo, EdgeInfo> &graph,
    std::vector<int> *current_scc, bool *visited)
{
    visited[current] = true;

    // Add the vertex tto the strongly connected components vector.
    current_scc->push_back(current);
    
    for (int j = 0; j < graph[current].edges.size(); ++j)
        if (!visited[graph[current].edges[j].to])
            DFSPutIntoSCC(graph[current].edges[j].to, graph, current_scc, visited);
}

// Given a graph return a vector of the vectors, each containing vertices of one
// strongly connected component of this graph.
template <class VertexInfo, class EdgeInfo>
std::vector<std::vector<int>*> *StronglyConnectedComponents(Graph<VertexInfo, EdgeInfo> &graph)
{
    // Vector of vectors each containing vertices of one strongly connected component.
    // This will be returned as a result.
    std::vector<std::vector<int>*> *result = new std::vector<std::vector<int>*>;

    // vector to store vertexes in post order. Vertex is pushed onto the vertex once fully explored.
    std::vector<int> *post_ord = new std::vector<int>;

    // Allocate visited array and inicialize it.
    bool *visited = new bool[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i)
        visited[i] = false;
    
    // Iterate over the vertices of the graph...
    for (int i = 0; i < graph.Size(); ++i)
        // If vertex is not visited yet...
        if (!visited[i])
            // Traverse the graph in depth-first order and put vertex into post_order vector.
            DFSNumeratePostOrder(i, graph, post_ord, visited);
        
    // Clear visited array.
    for (int i = 0; i < graph.Size(); ++i)
        visited[i] = false;

    // Reverse the graph.
    graph.Reverse();

    // Traverse reversed graph. For every vertex in post order going from the end(!!!) od the vector
    // vertices that can be reached from it are in one strongly connected component.
    for (int i = post_ord->size()-1; i >= 0; --i)
        if (!visited[(* post_ord)[i]])
        {
            // Add to the result new vector that will store vertices IDs in the current SCC.
             result->push_back(new std::vector<int>);

            // All vertices reached form current vertex are in the same component.
            DFSPutIntoSCC((* post_ord)[i], graph, result->back(), visited);

            for (int i = 0; i < result->back()->size(); ++i)
                std::cout << (* result->back())[i] << " ";
            std::cout << "\n";
        }
    
    // Reverse graph back.
    graph.Reverse();

    // Delete allocated data strucutres.
    delete post_ord;
    delete[] visited;

    // Return the result.
    return result;
}   

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices adn edges need no more additional info.
struct Empty{ Empty() {}};

int main()
{
    Graph<Empty, Empty> G(9, directed);
    G.AddEdge(0, 8, Empty());
    G.AddEdge(2, 1, Empty());
    G.AddEdge(1, 7, Empty());
    G.AddEdge(7, 6, Empty());
    G.AddEdge(6, 2, Empty());
    G.AddEdge(6, 3, Empty());
    G.AddEdge(3, 5, Empty());
    G.AddEdge(5, 4, Empty());
    G.AddEdge(4, 3, Empty());
    G.AddEdge(8, 4, Empty());
    std::vector<std::vector<int>*> *scc = StronglyConnectedComponents(G);
    std::cout << "Strongly connected components of the graph:\n";
    for (int i = 0; i < scc->size(); ++i)
    {
        std::cout << "{ ";
        for (int j = 0; j < (* scc)[i]->size(); ++j)
            std::cout << (* ((* scc)[i]))[j]  << " ";
        std::cout << "}\n";
    }
    //Output:
    //  Strongly connected components of the graph:
    //  { 1 2 6 7 }
    //  { 0 }
    //  { 8 }
    //  { 4 5 3 }

    return 0;
}
*/