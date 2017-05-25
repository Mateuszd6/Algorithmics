/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Website: In progress.
Description: Graph library containing template graph class and graph utilities.
===============================================================================*/

#pragma once

#include <iostream>
#include <vector>

// An enum used to determine the type of the graph. When edge is added to directed graph, 
// eg. edge 1-3 it will be only possible to go to 3 form vertex 1. 
// Going in the opposite direction is not allowed unless edge 3-1 is added.
enum GraphType
{
	directed,
	undirected
};

// This is basic graph class. 
// EdgeInfo is a class for aditional edge info to store. 
// By default edges store info about two vertices that they connect. 
// VertexInfo is a class for aditional vertex info to store. 
// By defauly vertex store a list (std::vertor here) of all the edges going out from it.
// If no aditional info is required use simply empty struct.
// NOTE: Mulitedges in this graph are allowed, which means that it is possible 
//		 to have more than one edge connecting two vertices. 
template <class VertexInfo, class EdgeInfo>
class Graph
{
public:
	struct Edge : EdgeInfo
	{
		int from;
		int to;

		Edge() {} 
		Edge(EdgeInfo base_edge, int from, int to) : EdgeInfo(base_edge), from(from), to(to) {}
	};

	struct Vertex : VertexInfo
	{
		// Contains all edges from the vertex.
		std::vector<Edge> edges;

		Vertex() {}
		Vertex(VertexInfo base_vertex) : VertexInfo(base_vertex) {}
	};

private:
	// Maximum capacity of the graph. 
	// If requested more vertices on the constructor size will be set to this value.
	const static int MAX_GRAPH_SIZE = 1000000;

	// Size of the graph.
	int size;

	// Type of the graph (directed/undirected).
	GraphType graph_type;

	// Pointer to the array of vertices.
	Vertex *vertices;

public:
	// Returns the list (std::vector here) of edges comming out from given vertex. 
	// Idx must be less than the graph size.
	inline Vertex operator[] (unsigned int idx) { return vertices[idx]; }
	inline const Vertex operator[] (unsigned int idx) const { return vertices[idx]; }

	// Returns the size of the graph.
	inline int Size() { return size; }

	// Returns the type of the graph (directed/undirected).
	inline GraphType Type() { return graph_type; }

	// Adds edge between vertices A and B.
	void AddEdge(int A, int B, EdgeInfo base_edge)
	{
		if (Type() == directed)
		{
			vertices[A].edges.push_back(Edge(base_edge, A, B));
		}
		else if (Type() == undirected)
		{
			vertices[A].edges.push_back(Edge(base_edge, A, B));
			vertices[B].edges.push_back(Edge(base_edge, B, A));
		}
		else
			std::cerr << "Unspecyfied graph type.\n";
	}

	// Graph constructor, specify size and type (directed/undirected).
	Graph(int graph_size, GraphType type)
	{
		this->graph_type = type;
		if (graph_size > MAX_GRAPH_SIZE)
		{
			std::cerr << "Requested graph size is greater than maximum graph size which is " 
					<< MAX_GRAPH_SIZE << ".\n";
			std::cerr << "Graph size will be set to this value.\n";
			graph_size = MAX_GRAPH_SIZE;
		}
		this->size = graph_size;
		this->vertices = new Vertex[graph_size];
		// TODO: Should user be able to inicialize all the values with the given array of the vertex infos
	}

	// Graph destructor.
	~Graph()
	{
		delete[] vertices;
	}

private:
	// Used to print graph look in the console. Prints every edge for every vertex from 0 to graph_size-1. 
	// NOTE: The order of those edges is the order in which there were added to the graph.
	friend std::ostream& operator<<(std::ostream &os, const Graph<VertexInfo, EdgeInfo> &graph)
	{
		os << "Graph with " << graph.size << " vertices, ";
		graph.graph_type == directed ? os << "directed" : os << "undirected";
		os << ":\n";
		for (int i = 0; i < graph.size; ++i)
		{
			std::cout << i << ":  ";
			for (int j = 0; j < graph[i].edges.size(); ++j)
				std::cout << graph[i].edges[j].to << " ";
			std::cout << "\n";
		}
		return os;
	}
};
