/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Finds the largest clique of the given graph. Time complexity: O(2^n). Space complexity: O(2^n).
Notes:
       * It's 2^n algorithm (as clique problem is NP-complete) so the largest graph possible to proceed in 
         sensible time cannot have more than 30 vertices.
       * If there is more than one clique with the larges size only one of them is written to the given vector.
===============================================================================*/

#include <iostream>
#include <vector>
#include "Graph.cpp"
#include "..\Utility.cpp"

template <class VertexInfo, class EdgeInfo>
int Clique (Graph<VertexInfo, EdgeInfo> &graph, std::vector <int> *verticesInClique)
{
    // Graphs with more than 30 vertices cannot be computed in a reasonable time anyway.
    if (graph.Size() > 30)
    {
        std::cerr << "Graph is too big to solve the clique problem by brute force.";
        return 0;        
    }

    if (graph.Size() <= 1)
    {
        if (verticesInClique != NULL)
        {
            verticesInClique->clear();
            if (graph.Size() == 1)
                verticesInClique->push_back(0);
        }
        return graph.Size();
    }

    int result = 0;

    // Store graph as an array of ints, where vertices availabe from vertex i are stored in 
    // the edges_[i] as a bit mask. Original graph is used to create this array.
    int *edges_ = new int[graph.Size()]();
    for (int i = 0; i < graph.Size(); ++i)
        for (auto edge : graph[i].edges)
            edges_[edge.from] = (edges_[edge.from] | (1 << edge.to));
    
    // An array for dynamic programming. dp[i] indicates if vertex set stored as a bit mask in i
    // is a clique or not. We asume that empty graph is a clique.
    bool *dp = new bool[(1 << graph.Size())]();
    dp[0] = true;

    // Iterating over every set of the vertices using bit masks.
    for (int i = 1; i < (1 << graph.Size()); +
            // Calculate a size and chceck if n+i)
    {
        int first_vertex_in_set = utility::GetLowestBit(i);
        int current_vert_set = i;
       
        // Main set without one vertex.
        int sub_set = (current_vert_set ^ (1 << first_vertex_in_set));
        
        // Because of the way vertex sets are being procees, dp[sub_set] is already evaluated.
        // There are two conditions for the whole set to be clique.
        // Firstly set stored in sub_set must be clique because every clique sub set is also clique.
        // Secondly every sub_set vertex must have edge to first_vertex_in_set.
        if (dp[sub_set] && ((sub_set & edges_[first_vertex_in_set]) == sub_set))
        {ew largest clique was found.
            int set_size = utility::GetNumberOfSetBits(current_vert_set);
            if (set_size > result)
            {
                result = set_size;
                if (verticesInClique != NULL)
                {
                    verticesInClique->clear();
                    for (int j = 0; (1 << j) <= current_vert_set; ++j)
                        if ((current_vert_set & (1 << j)))
                            verticesInClique->push_back(j);
                }
            }
            dp[current_vert_set] = true;
        }
    }

    delete[] edges_;
    delete[] dp;
    
    return result;
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices and edges need no more additional info.
struct Empty { Empty() {} };

int main ()
{
    Graph<Empty, Empty> G(6, undirected);
    G.AddEdge(0, 1, Empty());   
    G.AddEdge(0, 3, Empty());   
    G.AddEdge(0, 4, Empty());   
    G.AddEdge(0, 5, Empty());   
    G.AddEdge(1, 2, Empty());   
    G.AddEdge(1, 3, Empty());   
    G.AddEdge(1, 4, Empty());   
    G.AddEdge(2, 3, Empty());   
    G.AddEdge(2, 4, Empty());   
    G.AddEdge(3, 4, Empty());   
    G.AddEdge(4, 5, Empty()); 
    std::vector<int> *foo = new std::vector<int>();
    std::cout << G << "Largest clique subset of the graph has the size "<< Clique(G, foo) << "\n"
        << "and vertices: ";
    for (auto i : (*foo))
        std::cout << i << " ";
    std::cout << "\n";
    //Output:
    //  Graph with 6 vertices, undirected:
    //  0:  1 3 4 5
    //  1:  0 2 3 4
    //  2:  1 3 4
    //  3:  0 1 2 4
    //  4:  0 1 2 3 5
    //  5:  0 4
    //  Largest clique subset of the graph has the size 4
    //  and vertices: 0 1 3 4
}
*/