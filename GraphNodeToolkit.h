#ifndef GRAPHNODETOOLKIT_H
#define GRAPHNODETOOLKIT_H
/** \file GraphNodeToolkit.h
	\brief Contains a Toolkit for the 'GraphNode' class.
	\author René H. Thomsen
	\date 22/04 -11
*/
// License: GPL
#include "GraphNode.h"
#include <vector>
#include <iostream>

/**
	\brief Toolkit for the 'GraphNode' class.
*/
template<typename Item>
class GraphNodeToolkit
{
    private:
    static char notUsed;
    // The function used for recursive calls by 'depthFirstSearch'
    static void dps(std::vector<GraphNode<Item>*> graph, GraphNode<Item>* startNode)
    {
        startNode->mark();
        std::cout << startNode->getLabel() << '\n';

        for(int i = 0; i < startNode->getEdges().size(); i++)
        {
            if(!(startNode->getEdges()[i]->isMarked()))
            {
                dps(graph, startNode->getEdges()[i]);
            }
        }
    }

	public:
	// Functions
	// -Constructors and destructor
	/// Default constructor
	GraphNodeToolkit()
	{

	}

	/// -Destructor
	~GraphNodeToolkit()
	{
	    // Nothing
	}

	// -Utility functions
	/** \brief Adds an edge
        \pre 'from' and 'to' are not NULL.
        \post Adds an edge to 'from'´s list of edges.
	*/
	static void addEdge(GraphNode<Item>* from, GraphNode<Item>* to)
	{
        from->addEdge(to);
	}

	/** \brief Removes an edge
        \pre 'from' and 'to' are not NULL.
        \post Removes an edge from 'from'´s list of edges.
	*/
	static void removeEdge(GraphNode<Item>* from, GraphNode<Item>* to)
	{
        from->removeEdge(to);
	}
	/** \brief Unmarks the nodes
        \pre 'nodes' is not NULL and neither are the nodes in it.
        \post Unmarks all the nodes in the vector
	*/
	static void unmarkAll(std::vector<GraphNode<Item>*> nodes)
	{
        for(int i = 0; i < nodes.size(); i++)
            nodes[i]->unMark();
	}

	/** \brief Print all connected
        \pre 'graph' and 'startNode' are not NULL and 'graph' contains all the nodes that 'startNode' is in a graph with.
        \post Prints out all the nodes 'startNode' is connected to in some kind of path.(All nodes gets unmarked)
	*/
	static void depthFirstSearch(std::vector<GraphNode<Item>*> graph, GraphNode<Item>* startNode)
	{
		unmarkAll(graph); // To make sure they are unmarked

        // Recursive searching
        dps(graph, startNode);

        // Cleaning
        unmarkAll(graph);
	}

    // -Helper functions
    private:
    static size_t nodeIndex(std::vector<GraphNode<Item>*> graph, GraphNode<Item>* target)
    {
        for(int i = 0; i < graph.size(); i++)
        {
            if(graph[i] == target)
                return(i);
        }
        return(-1); // Error
    }

    static size_t indexOfSmallestUnmarked(std::vector<GraphNode<Item>*> graph, std::vector<int> array /* Values for the nodes(Weights normally) */)
    {
        size_t smallIndex = 0;
        for(int i = 0; i < graph.size(); i++)
        {
            if(!graph[i]->isMarked())
            {
                smallIndex = i;
                break;
            }
        }
        int small = array[smallIndex];

        for(int i = 0; i < graph.size(); i++)
        {
            if(array[i] < small && array[i] != -1 && (!graph[i]->isMarked()))
            {
                smallIndex = i;
                small = array[smallIndex];
            }
        }

        return(smallIndex);
    }

    public:
    /** \brief Prints the shortest distance to the different nodes
        \pre 'graph' and 'startNode' are not NULL and 'graph' contains all the nodes that 'startNode' is in a graph with.
        \post Prints the shortest possible distance from 'startNode' to the other nodes in 'graph'.(All nodes gets unmarked)
	*/
	static void shortestDistance(std::vector<GraphNode<Item>*> graph, GraphNode<Item>* startNode)
	{
		unmarkAll(graph); // To make sure they are unmarked

	    std::vector<int> distances(graph.size());
	    for(int i = 0; i < graph.size(); i++)
            distances[i] = -1;

	    size_t startIndex = nodeIndex(graph, startNode);

	    distances[0] = 0;

	    for(int i = 0; i < graph.size() - 1; i++)
	    {
	        size_t nextIndex = indexOfSmallestUnmarked(graph, distances);
	        GraphNode<Item>* next = graph[nextIndex];
	        next->mark();

	        for(int i = 0; i < next->getEdges().size(); i++)
	        {
	            if(!next->getEdges()[i]->isMarked())
	            {
	                int sum = distances[nextIndex] + next->getWeights()[i];
	                if(distances[nodeIndex(graph, next->getEdges()[i])] > sum || distances[nodeIndex(graph, next->getEdges()[i])] == -1)
                        distances[nodeIndex(graph, next->getEdges()[i])] = sum;
	            }
	        }
	    }

		for(int i = 0; i < graph.size(); i++)
            std::cout << graph[i]->getLabel() << " :" << distances[i] << '\n';

		// Cleaning
        unmarkAll(graph);
	}

	/** \brief Prints the shortest path from one node to another.
        \pre 'graph' and 'startNode' are not NULL and 'graph' contains all the nodes that 'startNode' is in a graph with.
        \post Prints the shortest possible distance from 'startNode' to the other node.(All nodes gets unmarked)
	*/
	static void shortestPath(std::vector<GraphNode<Item>*> graph, GraphNode<Item>* startNode, GraphNode<Item>* targetNode)
	{
	    unmarkAll(graph); // To make sure they are unmarked

		// Variables + init
	    std::vector<int> distances(graph.size());
	    for(int i = 0; i < graph.size(); i++)
            distances[i] = -1;

	    size_t startIndex = nodeIndex(graph, startNode);

		std::vector<GraphNode<Item>*> prevNode(graph.size()); // Used to keep hold on who pointed to who in shortest path

	    distances[0] = 0;

	    for(int i = 0; i < graph.size() - 1; i++)
	    {
	        size_t nextIndex = indexOfSmallestUnmarked(graph, distances);
	        GraphNode<Item>* next = graph[nextIndex];
	        next->mark();

	        for(int i = 0; i < next->getEdges().size(); i++)
	        {
	            if(!next->getEdges()[i]->isMarked())
	            {
	                int sum = distances[nextIndex] + next->getWeights()[i];
	                if(distances[nodeIndex(graph, next->getEdges()[i])] > sum || distances[nodeIndex(graph, next->getEdges()[i])] == -1)
					{
                        distances[nodeIndex(graph, next->getEdges()[i])] = sum;
						prevNode[nodeIndex(graph, next->getEdges()[i])] = next;
					}
	            }
	        }
	    }

		// Print out
		// -Tmp variables
		std::cout << "Shortes path from '" << startNode->getLabel() << "' to '" << targetNode->getLabel() << "':\n";
		std::vector<GraphNode<Item>*> path;
		GraphNode<Item>* cursor = prevNode[nodeIndex(graph, targetNode)];

		// -Save path
		while(cursor != startNode)
		{
			path.push_back(cursor);
			cursor = prevNode[nodeIndex(graph, cursor)];
		}
		// -The printing
		std::cout << "Go from " << startNode->getLabel();
		for(int i = (path.size() - 1); i >= 0; i--)
            std::cout << " -> " << path[i]->getLabel();
        std::cout << " and finally to " << targetNode->getLabel() << '\n';

		// Cleaning
        unmarkAll(graph);
	}
};
#endif