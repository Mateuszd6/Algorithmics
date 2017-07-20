/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Finds the optimal graph bipartition. Time complexity: O(sqrt(V)*(2^V)). Space complexity: O(V).
Notes:
       * Works only for undirected graphs. The behaviour of the algorithm for directed graph is random.
===============================================================================*/

#include <iostream>
#include "Graph.cpp"

namespace detail
{
    // Recursive function to generate next bit set.
    template <class VertexInfo, class EdgeInfo>
    void BipartitionGenerateNextSet(Graph<VertexInfo, EdgeInfo> &graph, std::vector<bool> &sets, 
        int current_index, int number_of_1, int number_of_0, long long total_cost, long long &best_cost,
        std::vector<int> *best_partition_vertices)
    {
        static_assert(std::is_base_of<DistanceEdge, EdgeInfo>::value, 
            "EdgeInfo must inherit from DistanceEdge");

        // The end of recursion. Compute the value and return.
        if (number_of_0 == 0 || number_of_1 == 0)
        {
            if (total_cost < best_cost)
            {
                best_cost = total_cost;
                if (best_partition_vertices != NULL)
                {
                    best_partition_vertices->clear();
                    for(int i = 0; i < sets.size(); ++i)
                        if (sets[i])
                            best_partition_vertices->push_back(i);
                }
            }
            return;
        }

        // Search for first item to the right of the item with current_index,
        // with a different boolean value. Store it's index in the new_index variable.
        // It is guaranteed that this item exist, becasue otherwise previous if statement will be executed.
        int new_index = -1;
        for (int i = current_index+1; i < sets.size(); ++i)
        {   
            if (sets[i] != sets[current_index])
            {
                new_index = i;
                break;
            }
        }

        // assert(new_index >= 0);

        // Value of difference in cuted edges weight after changing current_index, and next_index.
        long long swap_cost_value = 0LL;

        // Function goes for recursion two times: with 0 as a current vertex and with 1.
        for (int i = 0; i < 2; ++i)
        {
            if (sets[current_index] == true)
                BipartitionGenerateNextSet(graph, sets, current_index+1, 
                    number_of_1-1, number_of_0, total_cost+swap_cost_value, best_cost, 
                    best_partition_vertices);
            else
                BipartitionGenerateNextSet(graph, sets, current_index+1, 
                    number_of_1, number_of_0-1, total_cost+swap_cost_value, best_cost, 
                    best_partition_vertices);
            
            // In first loop it swaps current_index and new_index.
            // In the second loop they are swapped back, so after this funtion returns
            // the state would be the same.
            for (auto edge : graph[current_index].edges)
            {
                if (sets[edge.to] == sets[current_index])
                    swap_cost_value += edge.weight;
                else
                    swap_cost_value -= edge.weight;
            }
            sets[current_index] = !sets[current_index];
            for (auto edge : graph[new_index].edges)
            {
                if (sets[edge.to] == sets[new_index])
                    swap_cost_value += edge.weight;
                else
                    swap_cost_value -= edge.weight;
            }
            sets[new_index] = !sets[new_index];
        }
    }

    // Calculate the first cut between inicial graph split set,
    template <class VertexInfo, class EdgeInfo>
    long long BipartitionCalculateInicialCost(Graph<VertexInfo, EdgeInfo> &graph, std::vector<bool> &sets)
    {
        static_assert(std::is_base_of<DistanceEdge, EdgeInfo>::value, 
            "EdgeInfo must inherit from DistanceEdge");
        
        long long result = 0;
        for (int i = 0; i < graph.Size(); ++i)
            for (auto edge : graph[i].edges)
                if (sets[edge.from] != sets[edge.to])
                    result += edge.weight;

        // Every edge was added twice so result is devided by 2.
        return (result / 2);
    }
}

// Get the lowest cost of edges that need to be cut to split a graph to two different parts.
template <class VertexInfo, class EdgeInfo>
long long BestGraphBipartition(Graph<VertexInfo, EdgeInfo> &graph, 
    std::vector<int> *best_partition_vertices)
{
    static_assert(std::is_base_of<DistanceEdge, EdgeInfo>::value, 
        "EdgeInfo must inherit from DistanceEdge");

    if (graph.Size() & 1)
        std::cout << "Graph with odd number of vertices cannot be bipartitioned.";

    // bool vector use to indicate vertex membership to the first or second set.
    std::vector<bool> sets(graph.Size());
    for (int i = 0; i < sets.size()/2; ++i)
        sets[i] = true;

    // Inicial cost is computed as passed as reference to the recurisve function.    
    long long inicial_cost = detail::BipartitionCalculateInicialCost(graph, sets);
    detail::BipartitionGenerateNextSet(graph, sets, 0, sets.size()/2, sets.size()/2, 
        inicial_cost, inicial_cost, best_partition_vertices);

    return inicial_cost;
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices need no more additional info.
struct Empty { Empty() {} };

int main()
{
	Graph<Empty, DistanceEdge> G(6, undirected);
	G.AddEdge(0, 1, DistanceEdge(1));
	G.AddEdge(0, 5, DistanceEdge(2));
	G.AddEdge(1, 2, DistanceEdge(1));
	G.AddEdge(1, 4, DistanceEdge(1));
	G.AddEdge(1, 5, DistanceEdge(3));
	G.AddEdge(2, 3, DistanceEdge(1));
	G.AddEdge(3, 4, DistanceEdge(2));
    G.AddEdge(4, 5, DistanceEdge(1));
    std::vector<int> *foo = new std::vector<int>();
    std::cout << G << "The best cost of splitting graph to two prarts is: " << BestGraphBipartition(G, foo) << "\n";
    std::cout << "In the first part there should be: ";
    for (auto i : (* foo))
        std::cout << i << " ";
    std::cout << "\n";
    //Output:
    //  Graph with 6 vertices, undirected:
    //  0:  1 5
    //  1:  0 2 4 5
    //  2:  1 3
    //  3:  2 4
    //  4:  1 3 5
    //  5:  0 1 4
    //  The best cost of splitting graph to two prarts is: 3
    //  In the first part there should be: 0 1 5

    return 0;
}
*/
