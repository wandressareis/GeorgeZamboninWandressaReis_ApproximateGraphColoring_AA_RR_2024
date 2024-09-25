#define _POSIX_C_SOURCE 199309L
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#include "utils.hpp"

#define BILLION 1000000000L

// A C++ program to implement the DSatur algorithm for graph
// coloring
 
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;
 
// A C++ program to implement the DSatur algorithm for graph
// coloring
 
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
using namespace std;
 
// Struct to store information
// on each uncoloured vertex
struct nodeInfo {
    int sat; // Saturation degree of the vertex
    int deg; // Degree in the uncoloured subgraph
    int vertex; // Index of vertex
};
struct maxSat {
    bool operator()(const nodeInfo& lhs,
                    const nodeInfo& rhs) const
    {
        // Compares two nodes by
        // saturation degree, then
        // degree in the subgraph,
        // then vertex label
        return tie(lhs.sat, lhs.deg, lhs.vertex)
               > tie(rhs.sat, rhs.deg, rhs.vertex);
    }
};
 
// Class representing
// an undirected graph
class Graph {
 
    // Number of vertices
    int n;
 
    // Number of vertices
    vector<vector<int>> adj;
 
public:
    // Constructor and destructor
    Graph(const vector<vector<int>>& inputGraph)
    {
        adj = inputGraph;
        n = adj.size(); // The size of the graph is determined by the input
    }
    ~Graph() { adj.clear(); }
 
    // Function to add an edge to graph
    void addEdge(int u, int v);
 
    // Colour the graph
    // using the DSatur algorithm
    vector<int> DSatur();
};
 
void Graph::addEdge(int u, int v)
{
    adj[u].push_back(v);
    adj[v].push_back(u);
}
 
// Assigns colors (starting from 0)
// to all vertices and
// prints the assignment of colors
vector<int> Graph::DSatur()
{
    int u, i;
    vector<bool> used(n, false);
    vector<int> c(n), d(n);
    vector<set<int> > adjCols(n);
    set<nodeInfo, maxSat> Q;
    set<nodeInfo, maxSat>::iterator maxPtr;
 
    // Initialise the data structures.
    // These are a (binary
    // tree) priority queue, a set
    // of colours adjacent to
    // each uncoloured vertex
    // (initially empty) and the
    // degree d(v) of each uncoloured
    // vertex in the graph
    // induced by uncoloured vertices
    for (u = 0; u < n; u++) {
        c[u] = -1;
        d[u] = adj[u].size();
        adjCols[u] = set<int>();
        Q.emplace(nodeInfo{ 0, d[u], u });
    }
 
    while (!Q.empty()) {
 
        // Choose the vertex u
        // with highest saturation
        // degree, breaking ties with d.
        // Remove u from the priority queue
        maxPtr = Q.begin();
        u = (*maxPtr).vertex;
        Q.erase(maxPtr);
 
        // Identify the lowest feasible
        // colour i for vertex u
        for (int v : adj[u])
            if (c[v] != -1)
                used[c[v]] = true;
        for (i = 0; i < used.size(); i++)
            if (used[i] == false)
                break;
        for (int v : adj[u])
            if (c[v] != -1)
                used[c[v]] = false;
 
        // Assign vertex u to colour i
        c[u] = i;
 
        // Update the saturation degrees and
        // degrees of all uncoloured neighbours;
        // hence modify their corresponding
        // elements in the priority queue
        for (int v : adj[u]) {
            if (c[v] == -1) {
                Q.erase(
                    { int(adjCols[v].size()), d[v], v });
                adjCols[v].insert(i);
                d[v]--;
                Q.emplace(nodeInfo{ int(adjCols[v].size()),
                                    d[v], v });
            }
        }
    }
 
    // The full graph has been coloured.
    // Print the result
    for (u = 0; u < n; u++)
        cout << "Vertex " << u << " --->  Color " << c[u]
             << endl;

    return c;
}
int main(int argc, char* argv[]){
    char filename[300], order, algorithm_name[] = "dsatur";
    
    get_path_filename(filename, argv[1]);
    
    string filepath = argv[1]; // Provide the .col file path

    vector<vector<int>> graph;
    int numVertices, numEdges;

    // Step 1: Read the graph from the .col file
    readGraph(filepath, graph, numVertices, numEdges);
    Graph graph_new(graph);

    // Step 2: Apply the First-Fit Greedy Coloring algorithm

    // measure execution time:
    uint64_t diff;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    vector<int> colors = graph_new.DSatur();
    clock_gettime(CLOCK_MONOTONIC, &end);

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

    // Step 3: Output the result
    cout << "Vertex Coloring:\n";
    for (int i = 0; i < colors.size(); i++) {
        cout << "Vertex " << i + 1 << " ---> Color " << colors[i] << endl;
    }

    double max = *std::max_element(colors.begin(), colors.end());
    std::cout << "Max value: " << max << std::endl;
    write_csv(algorithm_name, filename, max, (long long unsigned int) diff);

    return 0;
}
