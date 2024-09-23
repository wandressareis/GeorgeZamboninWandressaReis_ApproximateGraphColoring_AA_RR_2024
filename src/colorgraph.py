from os import walk
import os
import os.path
import shlex
import subprocess
import logging
import pandas as pd

ALGO_DIR = "src/algorithms"

GRAPH_DIR = "graphs"

import re

import matplotlib.pyplot as plt
import networkx as nx

def extract_vertex_colors(file_content):
    # Regular expression to match the pattern 'Vertex n ---> Color k'
    pattern = r'Vertex (\d+) ---> Color (\d+)'
    
    # Create a list to store the colors
    vertex_colors = []
    
    # Find all matches in the file content
    matches = re.findall(pattern, file_content)
    
    # Populate the list with the colors at the correct vertex positions
    for match in matches:
        vertex = int(match[0])  # Vertex number (1-based)
        color = int(match[1])   # Color number
        
        # Adjust the list size if necessary
        while len(vertex_colors) < vertex:
            vertex_colors.append(None)  # Initialize with None
        
        # Assign the color to the corresponding vertex
        vertex_colors[vertex - 1] = color  # Vertex is 1-based, list is 0-based
    
    return vertex_colors


def create_graph_from_edges(num_vertices, edges):
    G = nx.Graph()
    G.add_nodes_from(range(num_vertices))
    G.add_edges_from(edges)
    return G


def load_dimacs_graph(file_path):
    edges = []
    num_vertices = 0

    with open(file_path, 'r') as file:
        for line in file:
            tokens = line.strip().split()

            # Ignore comments or empty lines
            if not tokens or tokens[0] == 'c':
                continue

            # Read the number of vertices and edges
            if tokens[0] == 'p':
                num_vertices = int(tokens[2])

            # Read the edges
            elif tokens[0] == 'e':
                vertex1 = int(tokens[1]) - 1  # Convert 1-based to 0-based indexing
                vertex2 = int(tokens[2]) - 1
                edges.append((vertex1, vertex2))

    return num_vertices, edges

def valid_color(graph, colored):
  for u in graph.nodes():
    for v in graph.neighbors(u):
        if colored[u] == colored[v]:
            return False
  return True


def color_graph():

    algorithms = []
    for file in os.listdir(ALGO_DIR):
        if len(file.split('.')) == 1:
            algorithms.append(file)

    selected_algo = algorithms[int(input("\nSelect the algorithm:\n {}algo (n): ".format("".join([f"\t{i} - {name}\n" for i, name in enumerate(algorithms)]))))]
    BINARY_PROGRAM = ALGO_DIR + os.path.sep + selected_algo

    graphs = []
    for file in os.listdir(GRAPH_DIR):
        graphs.append(file)

    graph_algo = graphs[int(input("\nSelect the graph:\n {}graph (n): ".format("".join([f"\t{i} - {name}\n" for i, name in enumerate(graphs)]))))]
    INPUT = GRAPH_DIR + os.path.sep + graph_algo

    if not os.path.exists(BINARY_PROGRAM):
        print(f"Input file: {BINARY_PROGRAM} not found")
    else:
        print("./" + BINARY_PROGRAM + " " + INPUT)
        cmd = shlex.split("./" + BINARY_PROGRAM + " " + INPUT)
        process = subprocess.Popen(cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                universal_newlines=True)
        stdout, stderr = process.communicate()
        if not stderr:
            colors = extract_vertex_colors(stdout)
            
            # Example usage:
            num_vertices, edges = load_dimacs_graph(INPUT)
            G = create_graph_from_edges(num_vertices, edges)
            
            print(f"is colors valid? {valid_color(G, colors)}.")

            print(colors)
            # Use spring_layout with a larger k to increase node separation
            pos = nx.spring_layout(G, k=0.5, iterations=50)
            
            # Draw the nodes with reduced node size and different color map
            nx.draw_networkx_nodes(G, pos, node_color=colors, node_size=100, cmap=plt.cm.Set1)

            # Draw edges with reduced thickness and transparency
            nx.draw_networkx_edges(G, pos, alpha=0.3, width=0.5)

            # Draw labels with smaller font size, or remove it completely for clarity
            nx.draw_networkx_labels(G, pos, font_size=6, font_color='black')
            
            # Remove axis
            plt.axis('off')
            
            # Show the plot
            plt.show()

            