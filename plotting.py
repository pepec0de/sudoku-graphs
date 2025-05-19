# -*- coding: utf-8 -*-

"""
requirements: GraphViz lib

$ sudo apt install graphviz
$ pip install graphviz
"""

import graphviz
import math

# Function to convert a string representation of an adjacency matrix to a 2D integer list
def string_to_adjacency_matrix(labels_str, matrix_str):
    # Split the string by lines, then by spaces to create the 2D list of integers
    labels = list(map(int, labels_str.split(',')))
    matrix = [list(map(int, row.split())) for row in matrix_str.strip().split('\n')]
    return labels, matrix

"""
# old: adjacency_matrix_to_grid_dot() wrong style
def adjacency_matrix_to_grid_dot(labels, adj_matrix):
    dot = graphviz.Graph()
    num_nodes = len(adj_matrix)
    grid_size = int(num_nodes ** 0.5)  # Assuming a roughly square layout, like 5x5

    # Add nodes and arrange them in a grid pattern
    for i in range(num_nodes):
        # Position nodes in a grid
        dot.node(str(i), shape="circle", label=str(labels[i]), pos=f"{i % grid_size},{-(i // grid_size)}!")

    # Add edges based on the adjacency matrix
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):  # Avoid duplicating edges in undirected graph
            if adj_matrix[i][j] == 1:
                dot.edge(str(i), str(j))
    
    # Set graph attributes for layout and fixed positioning
    dot.attr(overlap="scale", splines="true")  # Improve edge routing
    dot.attr(layout='neato')  # Use 'neato' layout engine
    dot.attr(nodesep = '0.5')
    dot.attr(rankdir = 'LR')
    
    return dot
"""

def adjacency_matrix_to_grid_dot(labels, adj_matrix, spacing=1.5):
    dot = graphviz.Graph(engine='neato')  # Usamos 'neato' para posicionar con coordenadas

    num_nodes = len(adj_matrix)
    grid_size = math.ceil(num_nodes ** 0.5)

    # Añadir nodos con mayor separación
    for i in range(num_nodes):
        x = (i % grid_size) * spacing
        y = -(i // grid_size) * spacing
        dot.node(str(i),
                 label=str(labels[i]),
                 shape="circle",
                 pos=f"{x},{y}!",
                 width="0.5",
                 height="0.5",
                 fixedsize="true")

    # Añadir aristas
    for i in range(num_nodes):
        for j in range(i + 1, num_nodes):
            if adj_matrix[i][j] == 1:
                dot.edge(str(i), str(j))

    # Parámetros del grafo
    dot.attr(overlap="scale")  # Permite que el grafo se escale para evitar solapamientos
    dot.attr(splines="true")   # Curvas suaves para las aristas

    return dot

def plot_graph(labels, adj_mat):
    # Convert to adjacency matrix
    labels, adj_matrix = string_to_adjacency_matrix(labels, adj_mat)

    # Generate and render the graph
    dot = adjacency_matrix_to_grid_dot(labels, adj_matrix)
    dot.render("grid_graph", format="png", view=True)

"""
Use of plot_graph() func

labels should be string vector like:
(N=3): 
labels = "1,2,3,2,3,1,3,1,2"
and 
adj_matrix = ""
0 0 0 0 0 0 1 0 0
0 0 0 0 1 0 1 0 0
0 0 0 0 0 1 0 1 1
0 0 0 0 1 0 1 0 0
0 1 0 1 0 0 0 0 1
0 0 1 0 0 0 0 0 0
1 1 0 1 0 0 0 0 0
0 0 1 0 0 0 0 0 0
0 0 1 0 1 0 0 0 0
""
"""