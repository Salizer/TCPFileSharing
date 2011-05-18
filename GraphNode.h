#ifndef GRAPHNODE_H
#define GRAPHNODE_H
/** \file  GraphNode.h 
	\brief Contains a class for a Node(vertex) to be used in graphs.
	\author René H. Thomsen
	\date 22/04 -11
*/
// License: GPL
#include <vector> // Used to contain edges


/** \brief Class that implements a node which can be used for directed graphs and the label can be of user chosen type.
    \warning Functions have not been implemented as 'const' so doesn´t work on constant nodes.
*/
template<typename Item>
class GraphNode
{
	private:
	// Variables
    std::vector<GraphNode*> edges; // Pointer to nodes which node points to
    std::vector<unsigned char> weights; // Weights for the edges
    Item label; // Label for the node
    bool marked; // Used for search algorithms and etc.

	public:
	// Functions
	// -Constructors and destructor
	/// Default constructor
	GraphNode(const Item argLabel)
	{
        // Set start values
        label = argLabel;
        marked = false;
	}
	/// Destructor
	~GraphNode()
	{
	    // Nothing
	}

	// -Utility functions
	/// Adds an edge to the node
	void addEdge(GraphNode* otherNode)
	{
        edges.push_back(otherNode);
        weights.push_back(0);
	}
	/// Adds an edge to the node with weight
	void addEdge(GraphNode* otherNode, unsigned char weight)
	{
        edges.push_back(otherNode);
        weights.push_back(weight);
	}

	/// Removes and edge from the node
	void removeEdge(GraphNode* otherNode)
	{
        for(int i = 0; i < edges.size(); i++)
        {
            if(edges[i] == otherNode)
            {
                edges.erase(edges.begin() + i);
                weights.erase(weights.begin() + i);
                break; // Only erases one occurrence
            }
        }
	}

	/// Changes the label to the copy of the argument
	void changeLabel(const Item newLabel)
	{
        label = newLabel;
	}

	/// Returns 'true' if the node is marked
	bool isMarked()
	{
	    return(marked);
	}

	/// Sets the node as marked('true')
    void mark()
    {
        marked = true;
    }

    /// Unmarks the node('false')
    void unMark()
    {
        marked = false;
    }

    /// Returns the list of edges
    std::vector<GraphNode*>& getEdges()
    {
        return(edges);
    }

    /// Returns the label value
    const Item& getLabel()
    {
        return(label);
    }

    /// Returns the weights for the edges
    const std::vector<unsigned char>& getWeights()
    {
        return(weights);
    }
};
#endif