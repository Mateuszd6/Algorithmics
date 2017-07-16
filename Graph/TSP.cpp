/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Travelling salesman problem using dynamic programming. Time complexity: O((n^2)*(2^n)). Space complexity: O(2^n).
Notes:
       * Finds the shortest Hamiltonian cycle in the graph.
       * Input graph must be directed.
===============================================================================*/

#include <iostream>
#include "Graph.cpp"

namespace detail
{
    // 
    struct BestRoadReturnValue
    {
        long long road_length;
        int prev_vertex;
    };

    BestRoadReturnValue BestRoad(int destination, int vertex_set, int **distance_matrix, 
        long long** best_road)
    {
        BestRoadReturnValue res;
        res.road_length = _I64_MAX;
        res.prev_vertex = -1;
        
        if (vertex_set == 0)
        {
            res.road_length = distance_matrix[0][destination];
            res.prev_vertex = 0;
            return res;
        }

        for (int j = 0; (vertex_set >> j); ++j)
            if (vertex_set & (1 << j))
            {
                ;//j+1 in the set.
                // If there is no edge between last vertex and destination one ignore this posibility.
                if (distance_matrix[j+1][destination] == INT_MAX)
                    continue;

                // Bit mask repersents previous set of verices excluding one with number j+1,
                // which is a candidate for the 'previous vertex'. 
                int new_set = vertex_set ^ (1 << j);

                long long current_road_weight = 0LL + distance_matrix[j+1][destination] + 
                    best_road[j+1][new_set];

                if (current_road_weight < res.road_length)
                {
                     res.road_length = current_road_weight;
                     res.prev_vertex = j+1;
                }  
            }

        
        return res;
    }
}

// Calculate cost of the travelling salesman road. (The cheapest Hamiltonian cycle).
template <class VertexInfo, class EdgeInfo>
long long GetTSP(Graph<VertexInfo, EdgeInfo> &graph, int *cycle)
{
	static_assert(std::is_base_of<DistanceEdge, EdgeInfo>::value, 
        "EdgeInfo must inherit from DistanceEdge");
    // assert(graph.Type() == directed);

    // Transfor the given graph to the distance matrix.
    int **distance_matrix = new int*[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i)
    {
        distance_matrix[i] = new int[graph.Size()];
        for (int j = 0; j < graph.Size(); ++j) 
            distance_matrix[i][j] = INT_MAX;
    }    
    for (int i = 0; i < graph.Size(); ++i)
        for (auto edge : graph[i].edges)
        {
            if (edge.from == edge.to)
                continue;

            if (distance_matrix[edge.from][edge.to] == INT_MAX)
                distance_matrix[edge.from][edge.to] = edge.weight;
            else
                distance_matrix[edge.from][edge.to] += edge.weight;

        }

    // Shortest road from every vertex to itself is set to 0.
    for (int i = 0; i < graph.Size(); ++i)
        distance_matrix[i][i] = 0;

    // best_road[i][j] indicated the weight of the shortest path to reach j 
    // by 0 going through vertices from the bit mask j. 
    long long **best_road = new long long*[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i)
        best_road[i] = new long long[1 << (graph.Size()-1)];

    // prev_vertex[i][j] stores what vertex in the set j was visited last (right before i).
    int **prev_vertex = new int*[graph.Size()];
    for (int i = 0; i < graph.Size(); ++i)
        prev_vertex[i] = new int[1 << (graph.Size()-1)];

    // Best way to 'reach' empty vertex set starting from 0 has total cost of 0.
    best_road[0][0] = 0LL;
    prev_vertex[0][0] = 0;

    // This loop generates all possible vertices set in range from 1 to graph.Size()-1.
    // NOTE: Vertex 0 is not in the bit masks.
    for (int i = 0; i < (1 << (graph.Size()-1)); ++i)
    {
        int bit_set = i;
        
        // For every vertex that is not in the bit mask...
        for (int j = 1; j < graph.Size(); ++j)
        {
            if (!(bit_set &(1<<(j-1))))
            {
                // Calculate the best way to j throught vertices in the bit mask.
                detail::BestRoadReturnValue roadResult = detail::BestRoad(j, bit_set, 
                    distance_matrix, best_road);
            
                best_road[j][bit_set] = roadResult.road_length;
                prev_vertex[j][bit_set] = roadResult.prev_vertex;
            }
        }
    }

    // final result is cost from going back to 0 throught all vertices in the graph.
    detail::BestRoadReturnValue result = detail::BestRoad(0, (1 << (graph.Size()-1))-1, 
        distance_matrix, best_road);
    
    // If cycle array was passed, fill it with vertices permutation in which they should be visited.
    // Use prev_vertex data to do it.
    if (cycle != NULL)
    {
        int idx = graph.Size()-1;
        int vert = result.prev_vertex;
        int vertex_set = (1 << (graph.Size()-1))-1;
        while (true)
        {
            cycle[idx--] = vert;
            
            if (idx < 0)
                break;

            vertex_set = (vertex_set ^ (1 << (vert-1)));
            vert = prev_vertex[vert][vertex_set];
        }

    }

    for (int i = 0; i < graph.Size(); ++i)
        delete[] distance_matrix[i];
    delete[] distance_matrix;

    for (int i = 0; i < graph.Size(); ++i)
        delete[] best_road[i];
    delete[] best_road;

    for (int i = 0; i < graph.Size(); ++i)
        delete[] prev_vertex[i];
    delete[] prev_vertex;

    return result.road_length;
}
//*
//============== USAGE EXAMPLE ===================

//Empty struct as edges and vertices need no more additional info.
struct Empty { Empty(){} } ;

int main()
{
    Graph<Empty, DistanceEdge> G(4, directed);
    G.AddEdge(0, 1, DistanceEdge(1));
    G.AddEdge(0, 2, DistanceEdge(15));
    G.AddEdge(0, 3, DistanceEdge(6));
    G.AddEdge(1, 0, DistanceEdge(2));
    G.AddEdge(1, 2, DistanceEdge(7));
    G.AddEdge(1, 3, DistanceEdge(3));
    G.AddEdge(2, 0, DistanceEdge(9));
    G.AddEdge(2, 1, DistanceEdge(6));
    G.AddEdge(2, 3, DistanceEdge(12));
    G.AddEdge(3, 0, DistanceEdge(10));
    G.AddEdge(3, 1, DistanceEdge(4));
    G.AddEdge(3, 2, DistanceEdge(8));
    int *foo = new int[G.Size()];
    std::cout << G << "Shortest Hamiltonian cycle in the graph has total weight of: " << 
        GetTSP(G, foo) << "\n(";
    for (int i = 0; i < G.Size()-1; ++i)
        std::cout << foo[i] << "->";
    std::cout <<foo[G.Size()-1] <<")\n";
    //Output:
    //  Graph with 4 vertices, directed:
    //  0:  1 2 3
    //  1:  0 2 3
    //  2:  0 1 3
    //  3:  0 1 2
    //  Shortest Hamiltonian cycle in the graph has total weight of: 21
    //  (0->1->3->2)
        
    return 0;
}
//*/