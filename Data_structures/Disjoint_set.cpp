/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Disjoit-set data structure also known as find-union. Allows for quick merging nodes into subset.
             Set of nodes with IDs from 0 to size-1, partitioned into a number of disjoint subsets. For each 
             subset there is a one node that represents the set. At start each node makes one-element set.
Operations:
	  * Find - find a ID of the set that given node belongs to. Time complexity: O(log*(n)).
      * Merge - given two nodes, if their sets are different merge them into one. Time complexity: O(log*(n)).
      * Add - add a new node which is also one-element set.  Time complexity: O(1).
      * Empty - returns true if there is no element in the structure. Time complexity: O(1).
	  * Clear - clear all elements in the structure. Time complexity: O(1).
      * Size - returns the size of the structure (the number of elements in). Time complexity: O(1).
	  * Capacity - returns the capacity of the structure - does not have to be same as size. Time complexity: O(1).
	  * MaxSize - returns maximum capacity of the structure. Time complexity: O(1).

Notes:
      * Made on std::vector it is also possible to add new nodes with Add function. New nodes is pushed to the back
        and if there where n elements before adding, its ID after is n.
      * Uses union-by-rank and path-compression heuristics.
	  * log* in the complexity is an iterated logarithm of n.
===============================================================================*/

#include <iostream>
#include <vector>

struct DisjointSetNode
{
    int id;
    int parent;
    int rank;

    DisjointSetNode() {}
    DisjointSetNode(int node_id)
    {
        id = node_id;
        parent = node_id;
        rank = 1;
    }
};

class DisjointSetArray
{
private:
    // Vector of the DisjointSetNode. Used instead of array, giving user an ability to resize by adding new nodes.
    std::vector <DisjointSetNode> nodes;

    // Vector used to write a traverse path when path compression algorithm is applied.
    std::vector < int > compressed_path;

    // Get parent of the node with index id in the array. When traversing apply path-compression algorithm.
    int GetParent (int id)
    {
        compressed_path.clear();

        // Start form the given vertex.
        int current_vertex = id;
       
        // Travere until there is a node which is its parent, write every node to the path vector.
        while (nodes[current_vertex].id != nodes[current_vertex].parent)
        {
            compressed_path.push_back(current_vertex);
            current_vertex = nodes[current_vertex].parent;
        }    
        
        // Now every vertex of the path points directly to the parent vertex.
        for (int i = 0; i < compressed_path.size(); ++i)
            nodes[i].parent = current_vertex;

        // Return the found vertex.
        return current_vertex;
    }

public: 
	// Returns the size of the nodes vector, which is the number of elements currently in the container.
	inline std::size_t Size() { return nodes.size(); }

	// Returns the capacity of the nodes vector.
	inline int Capacity() { return nodes.capacity(); } 

	// Returns the max size of the nodes vector.
	inline long long MaxSize() { return nodes.max_size(); }

    // Returns true is data strucutre is empty.
    inline bool Empty() { return nodes.empty(); }

    // Add new node to the vector.
    inline void Add() { nodes.push_back(DisjointSetNode(nodes.size())); }

    // Find a representation of the set that contains the node with given ID.
    int Find (int id) { return GetParent(id); }

    // Merge sets of two given nodes, if they aren't on the same set already.
    void Merge (int id1, int id2)
    {
        // Find set representations for both nodes.
        id1 = GetParent(id1);
        id2 = GetParent(id2);
        
        // If given nodes are not in the same set...
        if (id1 != id2)
        {
            // Apply union-by-rank algorithm: If one node has higher rank it will be a parent.
            // If ranks are same, increment rank of the first one and then merge.
            if (nodes[id1].rank == nodes[id2].rank)
                nodes[id1].rank++;
            if (nodes[id1].rank > nodes[id2].rank)
                nodes[id2].parent = id1;
            else if (nodes[id2].rank > nodes[id1].rank)
                nodes[id1].parent = id2;
        }
    }

    // Clear the array.
    void Clear() { nodes.clear(); }

	// Default constructor. Creates an empty array.
    DisjointSetArray ()
    {
        nodes = std::vector<DisjointSetNode>(0);
    }
    
    // Copy constructor.
    DisjointSetArray(const DisjointSetArray &other)
    {
        nodes = std::vector<DisjointSetNode>(other.nodes.size());
        for (int i = 0; i < nodes.size(); ++i)
            nodes[i] = other.nodes[i];
    }

    // Constructor used to create an array contains size nodes (with IDs from 0 to size-1).
    DisjointSetArray (int size)
    {
        nodes = std::vector<DisjointSetNode>(size);
        for (int i = 0; i < size; ++i)
        {
            // Manually assing the values for each node. Id is basicly an index, parent is same as id at start.
            // Rank is by default equal 1;
            nodes[i].id = i;
            nodes[i].parent = i;
            nodes[i].rank = 1;
        }
    }
};


/*
//============== USAGE EXAMPLE ===================

int main ()
{
    std::cout << "Creating disjoint set array with 8 elements...\n";
    DisjointSetArray *arr = new DisjointSetArray(8);
    std::cout << "Merging 1 and 2...\n";
    arr->Merge(1, 2);
    std::cout << "Merging 3 and 4...\n";
    arr->Merge(3, 4);
    std::cout << "Now:\n";
    for (int i = 1; i <= 4; ++i)
        std::cout << "Find(" << i << ") = " << arr->Find(i) << ".\n";
    std::cout << "Merging 2 and 4...\n";
    arr->Merge(2,4);
    std::cout << "Now:\n";
    for (int i = 1; i <= 4; ++i)
        std::cout << "Find(" << i << ") = " << arr->Find(i) << ".\n";
    arr->Merge(2,4);
    std::cout << "Adding new node (with index 8).\n";
    arr->Add();
    std::cout << "Merging 8 and 2...\n";
    arr->Merge(8, 2);
    std::cout << "Find(8) = " << arr->Find(8) << ".\n";
    //Output:
    //  Creating disjoint set array with 8 elements...
    //  Merging 1 and 2...
    //  Merging 3 and 4...
    //  Now:
    //  Find(1) = 1.
    //  Find(2) = 1.
    //  Find(3) = 3.
    //  Find(4) = 3.
    //  Merging 2 and 4...
    //  Now:
    //  Find(1) = 1.
    //  Find(2) = 1.
    //  Find(3) = 1.
    //  Find(4) = 1.
    //  Adding new node (with index 8).
    //  Merging 8 and 2...
    //  Find(8) = 1.



    return 0;
}
*/