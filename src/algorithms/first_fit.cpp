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

using namespace std;

// Function to apply First-Fit Greedy Coloring
vector<int> firstFitColoring(const vector<vector<int>>& graph) {
    int numVertices = graph.size();
    vector<int> result(numVertices, -1); // To store colors of vertices, initialized to -1
    result[0] = 0; // Assign the first color to the first vertex

    // Array to store the availability of colors
    vector<bool> available(numVertices, false);

    // Assign colors to remaining vertices
    for (int u = 1; u < numVertices; u++) {
        // Mark colors used by adjacent vertices
        for (int neighbor : graph[u]) {
            if (result[neighbor] != -1) {
                available[result[neighbor]] = true;
            }
        }

        // Find the first available color
        int color;
        for (color = 0; color < numVertices; color++) {
            if (!available[color]) {
                break;
            }
        }

        // Assign the found color
        result[u] = color;

        // Reset the availability for the next iteration
        for (int neighbor : graph[u]) {
            if (result[neighbor] != -1) {
                available[result[neighbor]] = false;
            }
        }
    }

    return result;
}

int main(int argc, char* argv[]){
    char filename[300], order, algorithm_name[] = "first_fit";
    
    get_path_filename(filename, argv[1]);
    
    string filepath = argv[1]; // Provide the .col file path

    vector<vector<int>> graph;
    int numVertices, numEdges;

    // Step 1: Read the graph from the .col file
    readGraph(filepath, graph, numVertices, numEdges);

    // Step 2: Apply the First-Fit Greedy Coloring algorithm

    // measure execution time:
    uint64_t diff;
    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    vector<int> colors = firstFitColoring(graph);
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
