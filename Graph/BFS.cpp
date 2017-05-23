/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: BFS algorithm. Time complexity: O(E+V). Space complexity: O(E+V).
Notes:
	  * This algorithm works for undirected graphs, as it doesn't metter from which
		vertex start, algorithm will always find every vertex in whole connected component.
	  * Algorith behavour for directed graphs is random.
	  * See usage example for details.
===============================================================================*/

#include <iostream>
#include <queue>
#include "Graph.h"


// Simple node containing the id of the vertex and the id of the parent through which it was reached.
struct BFSQueueNode
{
	int id;
	int parent;
	BFSQueueNode (int id, int parent) : id(id), parent(parent) {}
};

// Process one conected component of the graph with bfs algorithm.
// It takes a graph reference and a source vertex, which will be a root of the BFS tree.
// Function pointer is use to customize function used to process every vertexes.
// This function takes a BFSQueueNode (which contains info about current vertex and its parent.
// in_tree array stores info which vertex is in the tree and doesn't have to pe processed any more.
// If performing more than one BFS on the same graph (one BFS for every connected component),
// the array can be shared between BFS calls by passing pointer to it.
// If null passed fuction will create and inicialize its own in_tree array and delate it just before finishing.
// NOTE: The BFS tree will contain only a conected part of the graph in which source vertex exits.
// NOTE: In order to get a BFS tree of every conected component in the graph use BFSAllGraph.
template <class VertexInfo, class EdgeInfo>
void BFS(Graph<VertexInfo, EdgeInfo> &graph, int source, void (*ProcessVertex)(BFSQueueNode), bool *in_tree)
{
	// This is a queue to contain vertices potentially to process.
	std::queue < BFSQueueNode > bfs_queue;

	// Start by adding source vertex to the queue. This will be the root of bfs tree.
	// NOTE: When parent is negative it means there is none. It's because the source vertex is 
	//	     the root of the BFS tree.
	bfs_queue.push(BFSQueueNode(source, -1)); 

	// Passing in_tree array is used when more than one BFS is performed on the graph, so they share 
	// in_tree array. If NULL was passed temporary array will be created and then removed after search.
	bool in_tree_array_passed = (in_tree != NULL);
	
	// Create and inicialize array to store info about which vertices are already in the bfs tree.
	// If vertex is in the tree it won't be processed twice by the algorithm.
	if (!in_tree_array_passed)
	{
		in_tree = new bool[graph.Size()];
		for (int i = 0; i < graph.Size(); ++i)
			in_tree[i] = false;
	}
	// While bfs_queue is not empty:
	while (!bfs_queue.empty())
	{
		// Take the vertex on the top of the queue and remove it.
		int current_vertex = bfs_queue.front().id;
		int parent_vertex = bfs_queue.front().parent;
		bfs_queue.pop();

		// If it is already in the tree ignore it and continue the loop.
		if (in_tree[current_vertex])
			continue;

		// Otherwise add it to the BFS tree and add its neighbors to the queue.
		in_tree[current_vertex] = true;
		if (ProcessVertex != NULL)
			ProcessVertex(BFSQueueNode(current_vertex, parent_vertex));

		// Look for all neighbors of current_vertex.
		for (int i = 0; i < graph[current_vertex].edges.size(); ++i)
			// If a neighbor is not in the BFS tree yet, add it to the queue, 
			// as a vertex reached through current_vertex.
			if (!in_tree[graph[current_vertex].edges[i].to])
				bfs_queue.push(BFSQueueNode(graph[current_vertex].edges[i].to, current_vertex));
	}
	if (!in_tree_array_passed)
		delete[] in_tree;
}

// The same as passing NULL as in_tree to the BFS function above.
template <class VertexInfo, class EdgeInfo>
void BFS(Graph<VertexInfo, EdgeInfo> &graph, int source, void(*ProcessVertex)(BFSQueueNode))
{
	BFS(graph, source, ProcessVertex, NULL);
}

// Process every connected component of the graph with bfs algorithm.
// It takes a graph reference performs BFS search algorith number of connected components times.
// Function pointer is use to customize function used to process every vertexes.
// This function takes a BFSQueueNode (which contains info about current vertex and its parent.
template <class VertexInfo, class EdgeInfo>
void BFSAllGraph(Graph<VertexInfo, EdgeInfo> &graph, void(*ProcessVertex)(BFSQueueNode))
{
	// Create a global in_tree array shared by all BFS algorithm calls, and inicialize it.
	bool *in_tree = new bool[graph.Size()];
	for (int i = 0; i < graph.Size(); ++i)
		in_tree[i] = false;

	// Interate through vertices and if a given vertex is not in the tree, make it a root of BFS tree,
	// then start the BFS algorithm on the connected component..
	for (int i = 0; i < graph.Size(); ++i) 
		if (!in_tree[i]) 
			// With this method it is possible to pass in_tree array, to maintain info about visited vericies. 
			BFS(graph, i, ProcessVertex, in_tree);
	delete[] in_tree;
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as edges and vertices need no more additional info.
struct Empty { Empty(){} } ;

// Graph that has 11 vertices (from 0 to 10), undirected.
Graph<Empty, Empty> G(11, undirected);

void ExamplePrintBFSNode(BFSQueueNode node)
{
	if (node.parent == -1)
		std::cout << "ROOT " << node.id << ":\n";
	else
		std::cout << node.parent << " -> " << node.id << "\n";
}

int main()
{
	G.AddEdge(1, 3, Empty());
	G.AddEdge(2, 0, Empty());
	G.AddEdge(1, 0, Empty());
	G.AddEdge(4, 5, Empty());
	G.AddEdge(6, 8, Empty());
	G.AddEdge(3, 9, Empty());
	G.AddEdge(6, 4, Empty());
	G.AddEdge(7, 4, Empty());
	std::cout << G;
	std::cout << "\nBFS result:\n";
	
	BFSAllGraph(G, ExamplePrintBFSNode);
	// NOTE: this function outputs:
	//	ROOT 0:
	//	0 -> 2
	//	0 -> 1
	//	1 -> 3
	//	3 -> 9
	//	ROOT 4:
	//	4 -> 5
	//	4 -> 6
	//	4 -> 7
	//	6 -> 8
	//	ROOT 10:

	std::cout << "\n";

	BFS(G, 0, ExamplePrintBFSNode);
	// NOTE: this function outputs only:
	//	ROOT 0:
	//	0 -> 2
	//	0 -> 1
	//	1 -> 3
	//	3 -> 9

	return 0;
}
*/