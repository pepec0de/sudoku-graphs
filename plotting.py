# -*- coding: utf-8 -*-

"""
requirements: GraphViz lib

$ sudo apt install graphviz
$ pip install graphviz
"""

import graphviz
import math

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

def plot_graph(name, dir, labels, adj_mat):
    # Generate and render the graph
    dot = adjacency_matrix_to_grid_dot(labels, adj_mat)
    dot.render(filename=name, directory=dir, format="png", overwrite_source=True)

def plot_graphs_from_file(nombre_fichero):
    etiquetas = []
    matrices_todos = []

    with open(nombre_fichero, 'r') as f:
        lineas = f.readlines()

    i = 0
    cont = 0
    while i < len(lineas):
        # Buscar inicio del grafo
        if lineas[i].startswith("Unique"):
            # Leer etiquetas
            etiquetas_linea = lineas[i+1].strip().rstrip(',')
            if not etiquetas:
                etiquetas = list(map(int, etiquetas_linea.split(',')))

            n = len(etiquetas)
            matriz = [[0] * n for _ in range(n)]

            # Saltar dos líneas: la de etiquetas de nodos (1 2 3 4 ...) y pasar a la matriz
            i += 3
            for fila in range(n):
                linea = lineas[i + fila]
                partes = linea.split(':', 1)
                if len(partes) < 2:
                    continue
                valores = partes[1][1:]
                valores = valores.replace('  ', '0').replace(' ', '').strip()
                matriz[fila] = list(map(int, valores))

            matrices_todos.append(matriz)
            plot_graph(f"N{int(math.sqrt(n))}_unique_{cont+1}", "plots/", etiquetas, matriz)
            cont += 1
            
            i += n  # Saltar a la próxima sección
        else:
            i += 1

    return etiquetas, matrices_todos

"""
etiquetas, matrices_todos = read_graphs("n4_63_sols_unicas_10k_soluciones_con_hashCode.dat")

print(etiquetas)
print(len(matrices_todos))
"""

plot_graphs_from_file("n3.dat")