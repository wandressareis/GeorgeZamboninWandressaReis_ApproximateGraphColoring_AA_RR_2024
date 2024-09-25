#define _POSIX_C_SOURCE 199309L
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

#include "utils.hpp"
#include <stdexcept>

#define BILLION 1000000000L

using namespace std;

// Function to check if assigning color to node is valid
bool isValidColor(int node, int color, const vector<int>& colors, const vector<vector<int>>& graph) {
    for (int neighbor : graph[node]) {
        if (colors[neighbor] == color) {
            return false;  // Neighbor has the same color
        }
    }
    return true;
}

// Backtracking function to try coloring the graph
bool colorGraph(int node, int k, vector<int>& colors, const vector<vector<int>>& graph) {
    if (node == graph.size()) {
        return true;  // All nodes colored successfully
    }
    
    // Try all colors from 1 to k
    for (int color = 1; color <= k; ++color) {
        if (isValidColor(node, color, colors, graph)) {
            colors[node] = color;  // Assign color to the node
            
            if (colorGraph(node + 1, k, colors, graph)) {
                return true;  // Continue coloring the next node
            }
            colors[node] = 0;  // Backtrack, unassign the color
        }
    }
    return false;  // No valid coloring found with k colors
}

// Function to find the chromatic number
int findChromaticNumber(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<int> colors(n, 0);  // Color assignments for vertices
    int low = 1, high = n, chromaticNumber = n;
    
    while (low <= high) {
        int mid = (low + high) / 2;
        fill(colors.begin(), colors.end(), 0);  // Reset color assignments
        
        if (colorGraph(0, mid, colors, graph)) {
            chromaticNumber = mid;  // Found a valid coloring with mid colors
            high = mid - 1;         // Try to reduce the number of colors
        } else {
            low = mid + 1;          // Increase the number of colors
        }
    }
    
    return chromaticNumber;
}

int main(int argc, char* argv[]){
    char filename[300], order, algorithm_name[] = "chromaticNumber";
    
    get_path_filename(filename, argv[1]);
    
    string filepath = argv[1]; // Provide the .col file path

    vector<vector<int>> graph;
    int numVertices, numEdges;

    // Step 1: Read the graph from the .col file
    readGraph(filepath, graph, numVertices, numEdges);

    // if (numVertices > 100 || numEdges > 200) {
    //     throw std::invalid_argument("Compute time for current input exceed");
    // }

    // Step 2: Apply the First-Fit Greedy Coloring algorithm

    // measure execution time:
    uint64_t diff;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    //vector<int> colors = exactVertexColoring(graph);
    int chromatic_number = findChromaticNumber(graph);
    clock_gettime(CLOCK_MONOTONIC, &end);

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;

    // Step 3: Output the result
    // cout << "Vertex Coloring:\n";
    // for (int i = 0; i < colors.size(); i++) {
    //     cout << "Vertex " << i + 1 << " ---> Color " << colors[i] << endl;
    // }

    // double max = *std::max_element(colors.begin(), colors.end());
    // std::cout << "Max value: " << max << std::endl;
    write_csv(algorithm_name, filename, chromatic_number, (long long unsigned int) diff);
    printf("Chromatic number: %d\n", chromatic_number);

    return 0;
}