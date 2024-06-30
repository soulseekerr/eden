#pragma once

#include <iostream>
#include <vector>
#include <stack>

// Depth-First Search (DFS): 
// Explores as far as possible along each branch before backtracking.

// Use of Stack: The algorithm uses a stack to keep track of vertices to visit. 
// This implementation uses an explicit stack, but DFS can also be implemented 
// using recursion which implicitly uses the call stack.

// Graph Representation: The graph is represented using an adjacency list 
// which is a common and efficient way to represent sparse graphs.

// Graph class
class Graph {
public:
    Graph(int V); // Constructor

    void addEdge(int v, int w); // Function to add an edge to graph

    void DFS(int v); // DFS traversal from a given vertex

private:
    int nVertices_; // Number of vertices
    std::vector<std::vector<int>> adj_; // Adjacency list
};

Graph::Graph(int nVertices) {
    nVertices_ = nVertices;
    adj_.resize(nVertices_);
}

void Graph::addEdge(int v, int w) {
    adj_[v].push_back(w); // Add w to v’s list.
}

void Graph::DFS(int v) {
    std::vector<bool> visited(nVertices_, false); // Mark all the vertices as not visited
    std::stack<int> stack;

    // Push the current source node
    stack.push(v);

    while (!stack.empty()) {
        // Pop a vertex from stack and print it
        v = stack.top();
        stack.pop();

        // Stack may contain the same vertex twice. So, need to print the popped item only
        // if it is not visited.
        if (!visited[v]) {
            std::cout << v << " ";
            visited[v] = true;
        }

        // Get all adjacent vertices of the popped vertex v. If an adjacent has not been visited,
        // then push it to the stack.
        for (auto i = adj_[v].rbegin(); i != adj_[v].rend(); ++i) {
            if (!visited[*i])
                stack.push(*i);
        }
    }
}

void dfsExample() {
    // Create a graph
    Graph g(5); // 5 vertices numbered from 0 to 4

    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 2);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 3);
    g.addEdge(3, 4);

    /*
                      (0)
                     /  \
                   (1)-(2)
                       /
                     (3)
                     /
                   (4)
    */

    std::cout << "Depth First Traversal starting from vertex 2:" << std::endl;
    g.DFS(2);
}
