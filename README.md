# sudoku-graphs
Graph structures based on sudoku's concepts.

## TODO List

* ~~Check graph search algorithm results (for N=3 |graphs set| = 93 or |graphs set| = 208) compared with older versions (PRIORITY)~~.
* ~~Code memory reallocation for solutions vector (SudokuSet)~~.
* ~~Change the arguments to pass by reference when necessary (optimization in runtime)~~.
* ~~Implement graph search algorithm~~.
* ~~Add in GraphSet labels vector to remove valueOfCell function~~.
* ~~Implement sub-algo for binary sequences generation~~.
* Add code for graphical plots of graphs using GraphViz lib (Python script).
* ~~Add in GraphSet struct k_bits vector to optimize algorithm~~.

## List of ideas

* Implement a new type of data to effeciently store graphs (bitfields).
    * Think a way of storing graphs in files.
* Graph search algorithm: fill the rows with the lower degrees first in order to reduce the search tree at the top.
    * Concept: nCr(n, k to 1) is much lower than nCr(n, k to n).


Thanks to Antonio.

Simple graph search algorithm using backtracking:

```
function countValidGraphs(sudokuLabels):
    total_nodes = N * N
    degrees = array[total_nodes] filled with 0
    required_degrees = map each node -> its label from sudokuLabels
    adjacency = matrix[total_nodes][total_nodes] filled with 0
    return backtrack(0, 1, adjacency, degrees, required_degrees)

function backtrack(u, v, adjacency, degrees, required_degrees):
    if all degrees match required_degrees:
        return 1

    if u >= total_nodes:
        return 0
    if v >= total_nodes:
        return backtrack(u + 1, u + 2, adjacency, degrees, required_degrees)

    count = 0

    # Try adding edge (u, v) if valid
    if labels[u] ≠ labels[v] and degrees[u] < required_degrees[u] and degrees[v] < required_degrees[v]:
        add edge (u, v) to adjacency
        degrees[u] += 1
        degrees[v] += 1
        count += backtrack(u, v + 1, adjacency, degrees, required_degrees)
        remove edge (u, v)
        degrees[u] -= 1
        degrees[v] -= 1

    # Try without adding the edge
    count += backtrack(u, v + 1, adjacency, degrees, required_degrees)

    return count
```
