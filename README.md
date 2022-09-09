# Applications of metaheuristics inspired by nature in a specific optimization problem of a postal distribution sector

> **Note**<br>
> This repository contains a code used in the experiments conducted for the article written to [Applied Sciences](https://www.mdpi.com/journal/applsci), i.e. **Applications of metaheuristics inspired by nature in a specific optimization problem of a postal distribution sector** by [Michał Berliński](https://github.com/mberlins), [Eryk Warchulski](ewarchul.github.io), and [Stanisław Kozdrowski](orcid.org/0000-0001-6647-5189).

## About

This paper presents a logistics problem, related to the transport of goods, which can be applied in practice, for example in postal or courier services. Two mathematical models are presented as problems occurring in a logistics network. The main objective of the optimisation problem presented is to minimise capital resources (Capex), such as cars or containers. 

Three methods are proposed to solve this problem: a method based on mixed integer programming (MIP) and available through the CPLEX solver and two nature-inspired heuristic methods. The first is the evolutionary algorithm (EA) and the second is the artificial bee colony algorithm (ABC). 

Results indicate that the heuristics methods solve instances of large size, giving suboptimal solution and therefore enabling its application to real-life scenarios.

### Directory structure

* `src/ampl`

Contains implementation of the CPLEX solver in ampl.

* `src/cpp` 

Contains implementation of the ABC and the EA algorithms (cpp/Algorithms/)
together with the model implementation (cpp/Network/), i.e., the graph
structure which represents network of cities in considered logitic problem.

* `src/R`

Contains R functions used to generate plots depicted in the paper.


>Each implementation of considered solvers in the article are provided by
[Michał Berliński](https://github.com/mberlins) and were sligthly refactored by
[Eryk Warchulski](ewarchul.github.io).


## Requirements

* C++ compilator with C++17 support (e.g. `g++10`)
* `cmake` (>= v3.21.4)
* `ampl` (>= v20180618)
* `R` (>= v4.10, **optionally**, if you want to generate plots similar to these
    in the article).

## Build 

Run following commands to build C++ code:

```sh
mkdir -p src/cpp/build && cd src/cpp/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER='/path/to/compiler' .. 
make -j$(nproc)
```

It will build two shared library which provides the model of the cosidered
problem (`libModel.so`) and the algorithms (`libAlgorithms.so`) together with a
executable binary `NetworkSolver`.

## Numerical experiments reproduction

The usage of the `NetworkSolver` is following:

```
 ./NetworkSolver [--generate-dat] --method=[ABC|EA]
```
where:               
* `--generate-dat` generates a .dat file for the program in the AMPL language
* `--method` specifies which heuristics will be executed, i.e., the ABC or the
    EA.

To reproduce the experiments with the CPLEX solver, one has to run
`NetworkSolver` with `--generate-dat` option.

The generated file must be manually moved to the AMPL directory (`src/ampl`).

> **Note**<br>
> One can check that the names of the .model and .dat files match those given in the .run file. 

Then run the program by running the command:

```sh
ampl file.run
```
