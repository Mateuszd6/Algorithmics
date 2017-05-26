/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Dijkstra shortest-path algorithm using binary heap. Time complexity: O(E*log(E)). Space complexity: O(E+V).
Notes:
       * This algorithm builds the shortest-paths-tree over the given graph, finding the shortest path from given source
	 vertex to every verex reachable it.
       * Custom binary heap is used here, but std::priority_queue fits as well, but < operator must be redefined, since 
	 this containter is a max heap, when here min heap is needed.
       * This algorithm works for both type of graps: directed and undirected. 
===============================================================================*/

#include <iostream>
#include "Graph.cpp"
#include "..\Data_structures\Binary_heap.cpp"

// Class used by binary heap used to store info about which vertex is reachable with which cost.
// On the top of the priority queue there is always vertex with lowest reach cost.
// Only operator < is used by this data strucutre.
class DijkstraQueueNode
{
public:
	int reached_from;
	int vertex;
	int cost;

	DijkstraQueueNode () {}
	DijkstraQueueNode(int reached_from, int vertex, int cost) : 
		reached_from(reached_from), vertex(vertex), cost(cost) {}

	inline bool operator < (DijkstraQueueNode &other)
	{
		return cost < other.cost;
	}
};

// Returns a pointer to an array filled with cost of achieving given vertex. Cost of source is 0.
// Cost equal -1 means that vertex is not reachable through the source.
// Parent map is a poitner to the array specifying which vertex is reachable through which. 
// (Eg. Vertex i is reached through parent_map[i]).
// break_at_vertex is used when there is no need to search whole graph. 
// Function will stop and return incomplete array if given vertex is reached.
// If -1 passed function won't stop at any vertex.

template <class VertexInfo, class EdgeInfo>
int *DijkstraShortestPathTree(Graph<VertexInfo, EdgeInfo> &graph, 
	int source, int *parent_map, int break_at_vertex)
{
	static_assert(std::is_base_of<DistanceEdge, EdgeInfo>::value, "EdgeInfo must inherit from DistanceEdge");
	if (source < 0 || source >= graph.Size())
	{
		std::cerr << "Source out of range.\n";
		return NULL;
	}
	
	// Inicialize viisted array used to store info about which vertex was visited.
	bool *visited = new bool[graph.Size()];
	for (int i = 0; i < graph.Size(); ++i) visited[i] = false;
	
	// Inicialize result array:
	// -1 means vertex is not reachable.
	int *result = new int[graph.Size()];
	for (int i = 0; i < graph.Size(); ++i) result[i] = -1;

	// Inicialize the binary heap used as priorittty queue.
	BinaryHeap <DijkstraQueueNode> *heap = new BinaryHeap <DijkstraQueueNode>(minHeap);

	// Set bool indicating wheter or not parent map has to be filled.
	bool fill_parent_map = (parent_map != NULL);
	
	// Inicialize by passing the source vertex to the queue reached by nothing 
	// (-1 means no parent in the shortest-paths-tree).
	heap->Push(DijkstraQueueNode(-1, source, 0));

	// While there are still vertexes in prioritty queue...
	while (!heap->Empty())
	{
		// Take vertex with a best cost from heap, and pop it.
		DijkstraQueueNode heapTop = heap->Top();
		heap->Pop();

		// If it is already visited ignore and continue the while loop.
		if (visited[heapTop.vertex])
			continue;

		// Mark vertex visited and fill cost info and parent map if needed.
		visited[heapTop.vertex] = true;
		result[heapTop.vertex] = heapTop.cost;
		if (fill_parent_map)
			parent_map[heapTop.vertex] = heapTop.reached_from;
		
		// If algorithm was meant to break when reaching this vertex, break the while loop. 
		if (break_at_vertex == heapTop.vertex)
			break;

		// Then oterate over its neighbours...
		for (int i = 0; i < graph[heapTop.vertex].edges.size(); ++i)
			// If vertex is not visited yet...
			if (!visited[graph[heapTop.vertex].edges[i].to])
			{
				// Add it into the queue with cost of comming here + new edge cost:
				heap->Push(DijkstraQueueNode(heapTop.vertex, graph[heapTop.vertex].edges[i].to,
					heapTop.cost + static_cast<DistanceEdge>(graph[heapTop.vertex].edges[i]).weight));
			}
	}

	// Delate created data strucures.
	delete[] visited;
	delete heap;

	// Retur the result.
	return result;
}

// Calling dijkstra algorithm on whole graph, algorithm won't break after reaching any vertex.
// Same as passing -1 to the break_at_verex argument.
template <class VertexInfo, class EdgeInfo>
int *DijkstraShortestPathTree(Graph<VertexInfo, EdgeInfo> &graph, int source, int *parent_map)
{
	return DijkstraShortestPathTree(graph, source, parent_map, -1);
}

// Calling dijkstra algorithm without creating parent map. (Same as passing NULL in the parent_map argument).
template <class VertexInfo, class EdgeInfo>
int *DijkstraShortestPathTree(Graph<VertexInfo, EdgeInfo> &graph, int source, int break_at_vertex)
{
	return DijkstraShortestPathTree(graph, source, NULL, break_at_vertex);
}

// Calling dijkstra algorithm on whole graph, algorithm won't break after reaching any vertex, 
// and no parent map will be created.
// Same as passing -1 to the break_at_verex argument and NULL to the parent_map argument.
template <class VertexInfo, class EdgeInfo>
int *DijkstraShortestPathTree(Graph<VertexInfo, EdgeInfo> &graph, int source)
{
	return DijkstraShortestPathTree(graph, source, NULL, -1);
}

// Give the shortest path weight between vertexes A and B, pointer to the vector path can be specified
// to get the path vertices between A and B. If NULL passed path won't be created.
// Note that path vector is cleared before adding a path.
// If there is no path between vertices -1 is returned.
template <class VertexInfo, class EdgeInfo>
int ShortestPath(Graph<VertexInfo, EdgeInfo> &graph, int A, int B, std::vector < int> *path)
{
	bool construct_path = (path != NULL);
	if (construct_path)
		path->clear();

	// Handle the case when start is the same as meta.
	if (B == A)
	{
		// If path has to be constructed there is only vertex there.
		if (construct_path)
			path->push_back(A);
		return 0;
	}

	// Get parent map and cost array.
	int *parent_map = new int[graph.Size()];
	int *cost_arr = DijkstraShortestPathTree(graph, A, parent_map, B);
	
	// If B is unreachable form A return -1;
	if (cost_arr[B] == -1)
		return -1;

	// If path has to be constructed...
	if (construct_path)
	{
		// Start form B vertex.. 
		int curr_node = B;
		while (curr_node != A)
		{
			// Add vertex to the path list and move to its parent in the shortest-paths-tree.
			path->push_back(curr_node);
			curr_node = parent_map[curr_node];
		}

		// At the end add A node to the path.
		path->push_back(A);

		// And reverse the path because it was created backwards.
		std::reverse(path->begin(), path->end());
	}

	int result = cost_arr[B];

	delete[] cost_arr;
	delete[] parent_map;

	return result;
}

// Call shortest path between two vertices without constructing path.
// Same as passing NULL to the path argument.
template <class VertexInfo, class EdgeInfo>
int ShortestPath(Graph<VertexInfo, EdgeInfo> &graph, int A, int B)
{
	return ShortestPath(graph, A, B, NULL);
}

/*
//============== USAGE EXAMPLE ===================

//Empty struct as vertices need no more additional info.
struct Empty { Empty() {} };

int main()
{
	Graph<Empty, DistanceEdge> G(9, undirected);
	G.AddEdge(1, 2, DistanceEdge(3));
	G.AddEdge(3, 1, DistanceEdge(1));
	G.AddEdge(3, 2, DistanceEdge(1));
	G.AddEdge(5, 3, DistanceEdge(1));
	G.AddEdge(3, 6, DistanceEdge(7));
	G.AddEdge(0, 6, DistanceEdge(2));
	G.AddEdge(5, 0, DistanceEdge(4));
	G.AddEdge(0, 4, DistanceEdge(1));
	G.AddEdge(5, 4, DistanceEdge(2));
	G.AddEdge(7, 8, DistanceEdge(4));
	int *cost_arr = DijkstraShortestPathTree(G, 1, NULL, -1);
	std::cout << "Total cost array form vertex 1:\n";
	std::cout << "[vertex\tcost]\n";
	for (int i = 0; i < G.Size(); ++i)
		std::cout << "   " << i << "\t " << cost_arr[i] << "\n";

	std::vector<int> *path = new std::vector<int>();
	std::cout << "\nCost of going form 2 to 0 is " << ShortestPath(G, 2, 0, path) << " (";
	for (int i = 0; i < path->size(); ++i)
	{
		std::cout << (* path)[i];
		if (i != path->size() - 1) 
			std::cout << "->";
	}
	std::cout << ")\n";

	return 0;
}
*/
