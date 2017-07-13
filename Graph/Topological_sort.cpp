/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Topological sort algorithm using DFS. Time complexity: O(E+V). Space complexity: O(V).
Notes:
       * Probably the fastest topological sort algorithm.
       * Since the problem is related to the directed acyclic graphs, input graph must respect these requirements.
===============================================================================*/

#include <iostream> 
#include <vector>
#include "Graph.cpp"

namespace detail
{
    // DFS function. After vertex is fully explored in puts in in the result array based on exitTime value.
    // NOTE: ExitTime is passed by reference, because it exist outside DFS methods and must persist between them.
    //       After vertex is explored DFS call decrements its value by one, so precious calls will get the value properly.
    template <class VertexInfo, class EdgeInfo>
    void DFSTopologicalSort(int current, Graph<VertexInfo, EdgeInfo> &graph, int &exitTime, 
        bool *visited, int *topologicalSortResult)
    {
        visited[current] = true;
        for (int j = 0; j < graph[current].edges.size(); ++j)
            if (!visited[graph[current].edges[j].to])
                DFSTopologicalSort(graph[current].edges[j].to, graph, 
                    exitTime, visited, topologicalSortResult);
        
        topologicalSortResult[exitTime--] = current;
    }
}

// Return an array of the graph node indexes in such order
// that every edge goes to the verex which index is further in the array.
// NOTE: graph must be directed and acyclic for this to work properly.
template <class VertexInfo, class EdgeInfo>
int *TopologicalSort(Graph<VertexInfo, EdgeInfo> &graph)
{
    // Topological sort is a problem related to directed, acyclic graphs.
    assert(graph.Type() == directed);
    assert(graph.IsAcyclic());

    if (graph.Size() == 0)
        return NULL;

    // Result is an array to store the node indexes in the searched order.
    int *result = new int[graph.Size()]();

    // Visited array used by DFS.
    bool *visited = new bool[graph.Size()]();   
    
    // Index in which fully-explored vertex should be placed in the result array.
    int exitTime = graph.Size()-1;

    for (int i = 0; i < graph.Size(); ++i)
        if (!visited[i])
            detail::DFSTopologicalSort(i, graph, exitTime, visited, result);
    
    // Exit time started from graph.Size()-1, and was decremented by one for every vertex in the graph.
    // This assertion basically checks if every vertex as properly written to the output array.
    assert(exitTime == -1);

    delete[] visited;
    return result;
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices need no more additional info.
struct Empty { Empty() {} };

int main()
{
    Graph<Empty, Empty> *g = new Graph<Empty, Empty>(7, directed);
    g->AddEdge(0, 2, Empty());
    g->AddEdge(1, 3, Empty());
    g->AddEdge(3, 4, Empty());
    g->AddEdge(2, 4, Empty());
    g->AddEdge(3, 2, Empty());
    g->AddEdge(6, 0, Empty());
    g->AddEdge(5, 0, Empty());
    g->AddEdge(5, 1, Empty());
    std::cout << (* g) << "\nTopologically sorted vertices: ";
    int *sortedVertices = TopologicalSort(* g);
    for (int i = 0; i < g->Size(); ++i)
        std::cout << sortedVertices[i] << " ";
    // Output:
    //Graph with 7 vertices, directed:
    //0:  2
    //1:  3
    //2:  4
    //3:  4 2
    //4:
    //5:  0 1
    //6:  0
    //
    //Topologically sorted vertices: 6 5 1 3 0 2 4
}
*/