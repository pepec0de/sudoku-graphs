# sudoku-graphs
Graph structures based on sudoku's concepts.

## TODO List

* Check graph search algorithm results (for N=3 |graphs set| = 92) compared with older versions (PRIORITY).
* ~~Code memory reallocation for solutions vector (SudokuSet)~~.
* Change the arguments to pass by reference when necessary (optimization in runtime).
* ~~Implement graph search algorithm~~.
* ~~Add in GraphSet labels vector to remove valueOfCell function~~.
* ~~Implement sub-algo for binary sequences generation~~.
* Add code for graphical plots of graphs (GraphViz lib).
* Add in GraphSet struct k_bits matrix to optimize algorithm --> Test optimizations in graph search algorithm (PRIORITY).

## List of ideas

* Implement a new type of data to effeciently store graphs (bitfields).
    * Think a way of storing graphs in files.
* Graph search algorithm: fill the rows with the lower degrees first in order to reduce the search tree at the top.
    * Concept: nCr(n, k to 1) is much lower than nCr(n, k to n).


Thanks to Antonio.