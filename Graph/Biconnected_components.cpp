/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Find articulation points, bridges and biconnected components in the graph.
             Time complexity: O(E+V). Space complexity: O(E+V).
Notes:
       * This works only for undirected graph as all problems refere to these type of graphs.
       * All algorithms use low function calculated by SetLowFunction method.
       * Time and space complexity is same for every algorithm.
===============================================================================*/

#include <iostream>
#include <vector>
#include "Graph.cpp"
#include <unordered_map> // TODO: Implement myself.
#include "..\Utility.cpp"

namespace detail
{
    // Assing pre order and low values for every vertex in a connected component of a given graph.
    template <class VertexInfo, class EdgeInfo>
    void SetLowFunction (Graph<VertexInfo, EdgeInfo> &graph, int current, int from, int &time, int* pre_ord, int* low)
    {
        // Assing pre oder value and increment time variable;
        pre_ord[current] = time++;

        // Default low value is equal to the pre_order time of the vertex.
        low[current] = pre_ord[current];

        // Iterate current node neighbours...
        for (int i = 0; i < graph[current].edges.size(); ++i)
        {
            // Ignore DFS parent node.
            if (graph[current].edges[i].to == from)
                continue;

            // If the node is already visited update low value.
            if (pre_ord[graph[current].edges[i].to] != 0)
            {
                low[current] = utility::Min(low[current], pre_ord[graph[current].edges[i].to]);
                continue;
            }

            // If not visited yet, call the function recursively and upadte low value.
            SetLowFunction (graph, graph[current].edges[i].to, current, time, pre_ord, low);
            low[current] = utility::Min(low[current], low[graph[current].edges[i].to]);
        }
    }
}

// Get all articulation points from the given graph.
template <class VertexInfo, class EdgeInfo>
std::vector <int> *GetArticulationPoints(Graph<VertexInfo, EdgeInfo> &graph)
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

            // Call DFS function on the root.
            detail::SetLowFunction(graph, i, -1, time, pre_order, low);
            
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
                // This is the condition for articulation point. It must be true for at least one child.    
                if (low[graph[i].edges[j].to] >= pre_order[graph[i].edges[j].from])
                {
                    result->push_back(i);
                    break;
                }
    }
    
    // Delete allocated data structures.
    delete[] is_root;
    delete[] pre_order;
    delete[] low;
    
    // Return the result.
    return result;
}

// Get all articulation points from the given graph.
template <class VertexInfo, class EdgeInfo>
std::vector <typename Graph<VertexInfo, EdgeInfo> :: Edge> *GetBridges(Graph<VertexInfo, EdgeInfo> &graph)
{
    // The result vertor to store id's of articulation points of the graph.
    std::vector <typename Graph<VertexInfo, EdgeInfo> :: Edge> *result 
        = new std::vector <typename Graph<VertexInfo, EdgeInfo> :: Edge>(0);

    // Array to store preorder numbrer. 
    // pre_order == 0 means node has not been visited.
    int *pre_order = new int[graph.Size()]();
    
    // Time to determine when given node was reached by the algorithm.
    int time = 1;

    // Array to store low number.
    int *low = new int[graph.Size()];
    
    // Assing low function to every vertex in the graph.
    for (int i = 0; i < graph.Size(); ++i)
        // If vertex is not explored yet...
        if (pre_order[i] == 0)
            // Call DFS function on the root.
            detail::SetLowFunction(graph, i, -1, time, pre_order, low);

    // Iterate over every edge in the graph
    for (int i = 0; i < graph.Size(); ++i)
        for (int j = 0; j < graph[i].edges.size(); ++j)
            // First condition makes sure that every edge will be check only once,
            // second checks if given edge is a bridge, based on low function.
            if (pre_order[graph[i].edges[j].from] < pre_order[graph[i].edges[j].to] &&
                low[graph[i].edges[j].to] > pre_order[graph[i].edges[j].from])
                    // Add an edge to the result vector.
                    result->push_back(graph[i].edges[j]);
    
    // Delete allocated data structures.
    delete[] pre_order;
    delete[] low;
    
    // Return the result.
    return result;
}

// Get all biconnected components of the graph.
template <class VertexInfo, class EdgeInfo>
std::vector < std::vector < typename Graph<VertexInfo, EdgeInfo> :: Edge > *> *GetBiconnectedComponents(
    Graph<VertexInfo, EdgeInfo> &graph)
{
    std::vector < std::vector < typename Graph<VertexInfo, EdgeInfo> :: Edge > *> *result = 
        new std::vector < std::vector < typename Graph<VertexInfo, EdgeInfo> :: Edge > *>(0);
    
    // Array to store preorder numbrer. 
    // pre_order == 0 means node has not been visited.
    int *pre_order = new int[graph.Size()]();
    
    // Time to determine when given node was reached by the algorithm.
    int time = 1;

    // Array to store low number.
    int *low = new int[graph.Size()];

    // Assing low function to every vertex in the graph.
    for (int i = 0; i < graph.Size(); ++i)
        // If vertex is not explored yet...
        if (pre_order[i] == 0)
            // Call DFS function on the root.
            detail::SetLowFunction(graph, i, -1, time, pre_order, low);

    // Map to store low value into an index in the result array.
    std::unordered_map<int, int> *low_to_index = new std::unordered_map<int,int>();

    // Iterate over every edge in the graph
    for (int i = 0; i < graph.Size(); ++i)
        for (int j = 0; j < graph[i].edges.size(); ++j)
            // This condition ensures that evey edge will be checked only once.
            if (pre_order[graph[i].edges[j].from] < pre_order[graph[i].edges[j].to])
            {
                // The edge is a bridge which makes it seperate biconnected component.
                if (low[graph[i].edges[j].to] > pre_order[graph[i].edges[j].from])
                {
                    result->push_back(new std::vector <Graph<VertexInfo, EdgeInfo>::Edge>(0));
                    result->back()->push_back(graph[i].edges[j]);
                    continue;
                }
                
                // Otherwise low values of both ends of the edge are same.
                // Therefore, there is a biconnected component that every edge with this 
                // value on both ends belonds to.
                std::unordered_map<int,int>::const_iterator got = low_to_index->find(low[graph[i].edges[j].to]);
                if (got == low_to_index->end())
                {
                    // New biconnected component must be added to the result.
                    result->push_back (new std::vector <Graph<VertexInfo, EdgeInfo>::Edge>(0));
                    result->back()->push_back(graph[i].edges[j]);
                    low_to_index->insert(std::make_pair(low[graph[i].edges[j].to], (int)result->size()-1));
                }
                else
                {
                    // In this case the biconnected component with edges of this value was created, 
                    // so map is used to get its index.
                    result->operator[](got->second)->push_back(graph[i].edges[j]);
                }

            }

    // Delete allocated data structures.
    delete low_to_index;
    delete[] pre_order;
    delete[] low;
    
    // Return the result.
    return result;
}

/*
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
    std::vector <int> *articulation_points = GetArticulationPoints(g);
    std::cout << "Articulation points: ";
    for (auto i : (* articulation_points))
        std::cout << i << " ";
    std::vector < Graph<Empty, Empty> :: Edge> *bridges = GetBridges(g);
    std::cout <<"\nBridges: ";
    for (auto i : (* bridges))
        std::cout << i.from << "-" << i.to << " ";
    std::vector < std::vector <Graph<Empty, Empty> :: Edge > *> *
        biconnected_components = GetBiconnectedComponents(g);
    std::cout << "\nBiconnected components:\n";
    for (int i = 0; i < biconnected_components->size(); ++i)
    {
        std::cout << i+1 << ": ";
        for (auto j : (* (* biconnected_components)[i]))
            std::cout << j.from << "-" << j.to << " ";
        std::cout << "\n";
    }
    //Output:
    //  Articulation points: 0 15 1 2 3 5 8
    //  Bridges: 0-1 0-14 2-3 5-6 8-9 15-16 15-17
    //  Biconnected components:
    //  1: 0-1
    //  2: 0-14
    //  3: 1-2 1-13 2-7 7-8 8-10 10-11 10-12 10-13 11-12 12-13
    //  4: 2-3
    //  5: 3-4 3-5 4-5
    //  6: 5-6
    //  7: 8-9
    //  8: 15-16
    //  9: 15-17

    return 0;
}
*/