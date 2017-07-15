/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Dinic's algorithm for finding the max flow in the residual network. Time complexity: O(|E|*(|V|^2)). Space complexity: O(|V|^2).
Notes:
       * Works faster then Edmonds-Karp algorithm, and its avarege speed is to far for speed of O(n^3) algorithms.
       * Fully supports multiedges. Capacity is added when there are two edges with same begin and end.
===============================================================================*/

#include <iostream>
#include <queue>
#include "Graph.cpp"
#include "..\Utility.cpp"

namespace detail
{
    // Create a new layer graph using BFS. Returns true if t vertex was reached.
    template <class VertexInfo, class ResidualEdgeInfo>
    bool MaxFlowBFS(Graph<VertexInfo, ResidualEdgeInfo> &residualNetwork, int s, int t, 
        int *layers, int **capacity_matrix)
    {
        std::queue<int> bfsQueue;
        while (!bfsQueue.empty())
            bfsQueue.pop();

        // Start from the s vertex.        
        layers[s] = 0;
        bfsQueue.push(s);
        
        while (!bfsQueue.empty())
        {
            int currentNode = bfsQueue.front();
            bfsQueue.pop();

            for (auto edge : residualNetwork[currentNode].edges)
                
                // First condition means that the vertex hasn't beed reached already,
                // the second ensures that algorithm will ignore saturated edges. 
                if (layers[edge.to] == -1 && capacity_matrix[edge.from][edge.to] > 0)
                {
                    layers[edge.to] = layers[edge.from]+1;
                    bfsQueue.push(edge.to);
                }
        }

        return (layers[t] > 0);
    }

    // Try to reach the sink of the end of the network by using DFS.
    // Returns true if t vertex was reached.
    // NOTE: The DFS will move only through edges with capacity greater than 0
    //       and from the i'th layer to the i+1'th only.
    template <class VertexInfo, class ResidualEdgeInfo>
    bool MaxFlowDFs(int current, Graph<VertexInfo, ResidualEdgeInfo> &residualNetwork, int s, int t, 
        int *layers, std::vector<int> &path, int **capacity_matrix, int min_so_far, int &result)
    {
        // Push the vertex the the path holder.
        path.push_back(current);

        // If t was reached...
        if (current == t)
        {
            // Decrement all edges capacity on the path by the value of the smallest one.
            for (int i = 0; i < path.size()-1; ++i)
            {
                capacity_matrix[path[i]][path[i+1]] -= min_so_far;
                capacity_matrix[path[i+1]][path[i]] += min_so_far;
            }   

            // Add the smallest value to the result.
            result += min_so_far;
            path.pop_back();
            return true;
        }

        for (auto edge : residualNetwork[current].edges)
        {
            // Vertices in same layer or further layers than t will be ignored.
            if ((layers[edge.to] < layers[t] || edge.to == t) && layers[edge.from]+1 == layers[edge.to] 
                && capacity_matrix[edge.from][edge.to] > 0)
            {
                // If true is returned this means an end was found and algorithm must stop its work.
                // So basically if any call returns true, this one also does.
                if (MaxFlowDFs(edge.to, residualNetwork, s, t, layers, path, capacity_matrix, 
                    utility::Min(min_so_far, capacity_matrix[edge.from][edge.to]), result))
                {
                    return true;
                }
            }
        }
        
        // If nothing returned true this means it is impossible to reach the t vertex 
        // through the current one. Vertex is poped from the path and false is returned.
        path.pop_back();
        return false;
    }
}

// Given a residual network, returns a value of the max flow in it. 
template <class VertexInfo, class ResidualEdgeInfo>
int MaxFlow(Graph<VertexInfo, ResidualEdgeInfo> &residualNetwork, int s, int t)
{
    // capacity info is required to build the capacity_matrix matrix.
    // It is never used later in the algorithm.
	static_assert(std::is_base_of<ResidualEdge, ResidualEdgeInfo>::value, 
        "ResidualEdgeInfo must inherit from ResidualEdge.");
    // assert(s < residualNetwork.Size());
    // assert(t < residualNetwork.Size());

    // Cost matrix for the whole graph. Graph cannot be too large because 
    // of the time complexity so storing the matrix isn't very expensive. 
    int **capacity_matrix = new int*[residualNetwork.Size()];
    for (int i = 0; i < residualNetwork.Size(); ++i)
        capacity_matrix[i] = new int[residualNetwork.Size()]();

    // Use the capacity value stored in the edges to fill the matrix.
    for (int i = 0; i < residualNetwork.Size(); ++i)
        for (auto edge : residualNetwork[i].edges)
            capacity_matrix[edge.from][edge.to] += edge.capacity;        
    
    // Layers array for BFS algorithm.
    int *layers = new int[residualNetwork.Size()];
    for (int i = 0; i < residualNetwork.Size(); ++i)
        layers[i] = -1;
        
    // Path vector to store the path in the DFS.
    std::vector<int> path;

    // Global result. It will be passed by reference to the DFS function.
    int result = 0;

    while (detail::MaxFlowBFS(residualNetwork, s, t, layers, capacity_matrix))
    {
        // Clear the path vector, because it might be filled by previous DFS call.
        path.clear();
        
        // The final return value can be ignored.
        while (detail::MaxFlowDFs(s, residualNetwork, s, t, layers, path, 
            capacity_matrix, INT_MAX, result));

        // Set default values in the layers array before BFS call.
        for (int i = 0; i < residualNetwork.Size(); ++i)
            layers[i] = -1;
    }

    return result;
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices need no more additional info.
struct Empty { Empty() {} };

int main()
{
    Graph <Empty, ResidualEdge> G(6, directed);
    G.AddEdge(0, 1, ResidualEdge(10));
    G.AddEdge(0, 2, ResidualEdge(10));
    G.AddEdge(1, 3, ResidualEdge(4));
    G.AddEdge(1, 2, ResidualEdge(2));
    G.AddEdge(1, 4, ResidualEdge(8));
    G.AddEdge(4, 3, ResidualEdge(6));
    G.AddEdge(2, 4, ResidualEdge(9));
    G.AddEdge(3, 5, ResidualEdge(10));
    G.AddEdge(4, 5, ResidualEdge(10));
    std::cout << G << "S is 0, T is 5\n";
    std::cout << "Max flow equals: " << MaxFlow(G, 0, 5) << "\n";

    //Output:
    //  Graph with 6 vertices, directed:
    //  0:  1 2
    //  1:  3 2 4
    //  2:  4
    //  3:  5
    //  4:  3 5
    //  5:
    //  S is 0, T is 5
    //  Max flow equals: 19
   
    return 0;
}
*/