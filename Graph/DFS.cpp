/*===============================================================================
Author: Mateusz Dudziñski.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: DFS algorithm. Time complexity: O(E+V). Space complexity: O(E+V).
Notes:
	  * This algorithm works for undirected graphs, as it doesn't metter from which
		vertex start, algorithm will always find every vertex in whole connected component.
	  * Algorith behavour for directed graphs is random.
	  * See usage example for details.
===============================================================================*/

#include <iostream>
#include "Graph.h"

// Function called recursively by the DFS algorithm. Used to traverse the graph.
// Note that storing parent info is not necessary, but provides additional info to the
// function used to process current vertex.
template <class VertexInfo, class EdgeInfo>
void DFSTraverse(Graph<VertexInfo, EdgeInfo> &graph, int current, int parent, void(*ProcessVertex)(int, int), bool *in_tree)
{
	// Add current vertex to the DFS tree and call the ProcessVertex function if pointer is not NULL.
	in_tree[current] = true;
	if (ProcessVertex != NULL)
		ProcessVertex(current, parent);

	// Look for all neighbour vertices of the current vertex and if it is not visited 
	// call the function recursively there.
	for (int i = 0; i < graph[current].edges.size(); ++i)
		if (!in_tree[graph[current].edges[i].to])
			DFSTraverse(graph, graph[current].edges[i].to, current, ProcessVertex, in_tree);
}

// Process one connected component of the graph with DFS algorithm.
// It takes a graph reference and a source vertex, which will be a root of the DFS tree.
// Function pointer is used to customize function used to process every vertex in the graph.
// This function takes two ints: current vertex and its parent vertex.
// in_tree array stores info which vertex is in the tree and doesn't have to pe processed any more.
// If performing more than one DFS on the same graph (one DFS for every connected component),
// the array can be shared between DFS calls by passing pointer to it.
// If null passed fuction will create and inicialize its own in_tree array and delate it just before finishing.
// The DFS algorithm will only visit the connected component of the graph in which source vertex is.
// In order to travere all graph with more than one DFS tree use DFSAllGraph.
template <class VertexInfo, class EdgeInfo>
void DFS(Graph<VertexInfo, EdgeInfo> &graph, int source, void(*ProcessVertex)(int, int), bool *in_tree)
{
	// Passing in_tree array is used when more than one DFS is performed on the graph, so they share 
	// in_tree array. If NULL was passed temporary array will be created and then removed after search.
	bool in_tree_array_passed = (in_tree != NULL);

	// Create and inicialize array to store info about which vertices are already in the DFS tree.
	// If vertex is in the tree it won't be processed twice by the algorithm.
	if (!in_tree_array_passed)
	{
		in_tree = new bool[graph.Size()];
		for (int i = 0; i < graph.Size(); ++i)
			in_tree[i] = false;
	}

	// Start recursive search function from the source vertex. 
	// parent == -1, means that the current vertex is a root of a DFS tree.
	DFSTraverse(graph, source, -1, ProcessVertex, in_tree);

	// Delete the in_tree array if it was created by this function (NULL was passed in the function call).
	if (!in_tree_array_passed)
		delete[] in_tree;
}

// The same as passing NULL as in_tree to the DFS function above.
// Used when only one connected component must be processed or all graph is connected.
template <class VertexInfo, class EdgeInfo>
void DFS(Graph<VertexInfo, EdgeInfo> &graph, int source, void(*ProcessVertex)(int, int))
{
	DFS(graph, source, ProcessVertex, NULL);
}

// Process every connected component of the graph with DFS algorithm.
// It takes a graph reference and performs depth-first search algorith number of connected components times.
// Function pointer is use to customize function used to process every vertexes.
// This function takes two ints (which contains info about current vertex and its parent).
template <class VertexInfo, class EdgeInfo>
void DFSAllGraph(Graph<VertexInfo, EdgeInfo> &graph, void(*ProcessVertex)(int, int))
{
	// Create a global in_tree array shared by all DFS algorithm calls, and inicialize it.
	// With this method it is possible to pass in_tree array, to maintain info about visited vericies. 
	bool *in_tree = new bool[graph.Size()];
	for (int i = 0; i < graph.Size(); ++i)
		in_tree[i] = false;

	// Interate through vertices and if a given vertex is not in the tree, make it a root of DFS tree,
	// then start DFS algorithm from this vertex.
	for (int i = 0; i < graph.Size(); ++i)
		if (!in_tree[i])
			DFS(graph, i, ProcessVertex, in_tree);

	// Delate in_tree array as it is not needed any more.
	delete[] in_tree;
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as edges and vertices need no more additional info.
struct Empty { Empty(){} } ;

// Graph that has 11 vertices (from 0 to 10), undirected.
Graph<Empty, Empty> G(11, undirected);

// This method takes a current vertex and its parent and its called once for every vertex visited in the DFS tree.
void ExampleProcessMethod(int current, int parent)
{
	// If parent is negative it means current vertex is a root of new DFS tree. 
	if (parent < 0)
		std::cout << "ROOT " << current << ":\n";
	// In another case current is reached in the tree through parent.
	else
		std::cout << parent << " -> " << current << "\n";
}

int main()
{
	G.AddEdge(0, 4, Empty());
	G.AddEdge(3, 2, Empty());
	G.AddEdge(7, 9, Empty());
	G.AddEdge(8, 9, Empty());
	G.AddEdge(4, 5, Empty());
	G.AddEdge(3, 0, Empty());
	G.AddEdge(0, 6, Empty());
	G.AddEdge(7, 10, Empty());
	G.AddEdge(8, 7, Empty());
	G.AddEdge(5, 2, Empty());
	std::cout << G << "\n";

	std::cout << "DFSAllGraph output:\n";
	DFSAllGraph(G, ExampleProcessMethod);
	//Output:
	//	ROOT 0:
	//	0 -> 4
	//	4 -> 5
	//	5 -> 2
	//	2 -> 3
	//	0 -> 6
	//	ROOT 1:
	//	ROOT 7:
	//	7 -> 9
	//	9 -> 8
	//	7 -> 10

	std::cout << "\n";

	std::cout << "DFS (source 0) output:\n";
	DFS(G, 0, ExampleProcessMethod);
	//Output:
	//	ROOT 0:
	//	0 -> 4
	//	4 -> 5
	//	5 -> 2
	//	2 -> 3
	//	0 -> 6

	return 0;
}
*/
