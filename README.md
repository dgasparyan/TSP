# TSP
Traveling Salesman Problem.

A simple framework to help solve Traveling Salesman Problem (TSP).

The main idea is that Solver class takes a collection of different (normally heuristic) algorithms, executes them in parallel 
and returns the Path with minimum distance.

New algoritms can be easly added by inheriting from PathFinder Interface and implementing pure virtual functions.

Optional optimization can be applied to the results of PathFinder by inheriting from and implementing PathOptimizer interface.

A couple of examples are provided, including an example of applying a 2-opt optimization to a path found by Nearest-neighbour insertion.
