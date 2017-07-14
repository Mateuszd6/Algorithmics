/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Finds an Euler cycle in one exist in directed or undirected Graph. Time complexity: O(E+V). Space complexity: O(E+V).
Notes:
       * Mulitedges are fully supported with this algorithm (see example for an example).
       * For undirected graph hash set is used (here std::unordered_set) to note which edge was traversed.
===============================================================================*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include "Graph.cpp"

namespace detail
{
    // Used to convert to int to an unsigned long long value. It can be used as a key in the hash set.
    unsigned long long GetPairID(int A, int B)
    {
        unsigned long long res = (((unsigned long long)A) << 32) + B;
        return res;
    }
}

// Check if euler cycle exist in the graph baes on vertices degrees and graph transposition (for directed).
template <class VertexInfo, class EdgeInfo>
bool ExistEulerCycle (Graph<VertexInfo, EdgeInfo> &graph)
{
    if (graph.Type() == directed)
    {           
        // Graph transposition.
        Graph<VertexInfo, EdgeInfo> *revGraph = new Graph<VertexInfo, EdgeInfo>(graph);
        revGraph->Reverse();

        bool result = true;

        // NOTE: Condition for Euler cycle to exist in directed graph: 
        //       For every vertex in-degree must equal out-degree.
        for (int i = 0; i < graph.Size(); ++i)
            if (graph[i].edges.size() != (* revGraph)[i].edges.size())
            {
                result = false;
                break;
            }

        delete revGraph;
        return result;
    }
    else if (graph.Type() == undirected)
    {
        // NOTE: Condition for Euler cycle to exist in undirected graph:
        //       Degree of every vertex must be even.
        for (int i = 0; i < graph.Size(); ++i)
            if (graph[i].edges.size()%2 != 0)
                return false;

        return true;
    }
    else
    {
        std::cerr << "Unrecognized graph type.";
        return false;
    }
}

// Return an Euler cycle in the graph. It is asumed that the cycle exist.
// Using recursive approach is not recommended, because there exist stack overflow potencial for large graphs.
template <class VertexInfo, class EdgeInfo>
std::vector <int> *EulerCycle (Graph<VertexInfo, EdgeInfo> &graph)
{
    if (graph.Size() <= 1)
        return NULL;

    // assert(ExistEulerCycle(graph));

    if (graph.Type() == directed)
    {           
        std::vector <int> *result = new std::vector<int>(); 

        // indexes[i] holds the next edge to explore index of the i'th vertex of the graph. 
        int *indexes = new int[graph.Size()]();

        // std::vector is used as a stack and is inicialized with first vertex of the graph.
        std::vector <int> stack;
        stack.push_back(0);

        // Non-recursive similar-to-DFS algorithm, but in can visit the same verex more than once. 
        while (!stack.empty())
        {
            int curr_top_value = stack.back();

            // All the edges comming out from the vertex were visited.
            // Vertex is poped from the stack and previous vertex is pushed to the output  vector. 
            if (indexes[curr_top_value] == graph[curr_top_value].edges.size())
            {
                stack.pop_back();

                if (!stack.empty())
                    result->push_back(stack.back());
            }

            // Next edge is getting explored.
            else
            {
                int next_to_proceed = graph[curr_top_value].edges[indexes[curr_top_value]++].to;    
                stack.push_back(next_to_proceed);
            }
        }

        // The output was created in the reversed order, so it must be reversed before returing.
        std::reverse(result->begin(), result->end());
        
        delete[] indexes; 
        
        return result;
    }
    else if (graph.Type() == undirected)
    {
        std::vector <int> *result = new std::vector<int>(); 

        // indexes[i] holds the next edge to explore index of the i'th vertex of the graph. 
        int *indexes = new int[graph.Size()]();

        // Hash set to store used edges.
        std::unordered_set<unsigned long long> uset;

        // std::vector is used as a stack and is inicialized with first vertex of the graph.
        std::vector <int> stack;
        stack.push_back(0);

        while (!stack.empty())
        {
            int curr_top_value = stack.back();

            // All the edges comming out from the vertex were visited.
            // Vertex is poped from the stack and previous vertex is pushed to the output  vector. 
            if (indexes[curr_top_value] == graph[curr_top_value].edges.size())
            {
                stack.pop_back();
                if (!stack.empty())
                    result->push_back(stack.back());
            }

            // Next edge is getting explored.
            else
            {
                int from = graph[curr_top_value].edges[indexes[curr_top_value]].from;
                int to = graph[curr_top_value].edges[indexes[curr_top_value]].to;

                // If edge was explored ignore it.
                if (uset.count(detail::GetPairID(from, to)))
                {
                    uset.erase(detail::GetPairID(from, to));
                    indexes[curr_top_value]++;
                }

                // Otherwise mark it as explored and use it.
                else
                {
                    uset.insert(detail::GetPairID(to, from));
                    indexes[curr_top_value]++;
                    stack.push_back(to);
                }
            }
        }

        // The output was created in the reversed order, so it must be reversed before returing.
        std::reverse(result->begin(), result->end());
        
        delete[] indexes;   
        
        return result;  
    }
    else
    {
        std::cerr << "Unrecognized graph type.";
        return NULL;
    }
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices and edges don't need any additional info.
struct Empty { Empty() {} };

int main ()
{
    Graph<Empty, Empty> *g = new Graph<Empty, Empty>(7, undirected);
    g->AddEdge(0, 2, Empty());
    g->AddEdge(2, 1, Empty());
    g->AddEdge(1, 0, Empty());
    g->AddEdge(6, 2, Empty());
    g->AddEdge(2, 3, Empty());
    g->AddEdge(3, 4, Empty());
    g->AddEdge(4, 5, Empty());
    g->AddEdge(5, 3, Empty());
    g->AddEdge(3, 6, Empty());
    std::vector<int> *cycle = EulerCycle(* g);
    std::cout << (* g);
    std::cout << "Euler cycle: ";
    for (auto i : (* cycle))
        std::cout << i << " ";
    std::cout << "\n\n";
    Graph<Empty, Empty> *h = new Graph<Empty, Empty>(4, undirected);
    h->AddEdge(0, 1, Empty());
    h->AddEdge(0, 1, Empty());
    h->AddEdge(0, 1, Empty());
    h->AddEdge(0, 1, Empty());
    h->AddEdge(2, 1, Empty());
    h->AddEdge(3, 2, Empty());
    h->AddEdge(1, 3, Empty());
    cycle = EulerCycle(* h);
    std::cout << (* h);
    std::cout << "Euler cycle: ";
    for (auto i : (* cycle))
        std::cout << i << " ";
    //Output:
    // Graph with 7 vertices, undirected:
    // 0:  2 1
    // 1:  2 0
    // 2:  0 1 6 3
    // 3:  2 4 5 6
    // 4:  3 5
    // 5:  4 3
    // 6:  2 3
    // Euler cycle: 0 2 6 3 4 5 3 2 1
    // 
    // Graph with 4 vertices, undirected:
    // 0:  1 1 1 1
    // 1:  0 0 0 0 2 3
    // 2:  1 3
    // 3:  2 1
    // Euler cycle: 0 1 0 1 2 3 1
}
*/