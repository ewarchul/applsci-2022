# applsci-2022

This repository contains a code used in the experiments conducted for the article written to Applied Sciences, i.e. 'Applications of metaheuristics inspired nature in combinatorial logistic problem' by Michał Berliński, [Eryk Warchulski](orcid.org/0000-0003-1416-7031), and [Stanisław Kozdrowski](orcid.org/0000-0001-6647-5189).

## About

This paper presents a logistics problem, related to the transport of goods, which can be applied in practice, for example in postal or courier services. Two mathematical models are presented as problems occurring in a logistics network. The main objective of the optimisation problem presented is to minimise capital resources (Capex), such as cars or containers. Three methods are proposed to solve this problem. The first is a method based on mixed integer programming (MIP) and available through the CPLEX solver. This method is the reference method for us because, as an exact method, it guarantees to find the optimal solution as long as the problem is not too large. However, the logistic problem under consideration belongs to the class of NP-complete problems and therefore, for larger problems, i.e., for networks of large size, the MIP method does not find any integer solution in a reasonable computational time. Therefore, two nature-inspired heuristic methods have been proposed. The first is the evolutionary algorithm and the second is the artificial bee colony algorithm. Results indicate that the heuristics methods solve instances of large size, giving suboptimal solution and therefore enabling its application to real-life scenarios.


## How to reproduce experiments

### Requirements

* cmake (>= 3.21.4)
* C++ compilator with C++17 support
* AMPL

### Build 

Run following commands to build C++ code:

```sh
mkdir -p src/cpp/build && cd src/cpp/build
cmake .. 
make -j$(nproc)
```

### Run experiments

After building the binaries of heuristics you are ready to reproduce numerical
experiments.

To reproduce heuristics numerical experiments run command below:

```
bash run-heuristics.sh
```

it will execute bash script and place all results in the `data/` directory.


To reproduce CPLEX experiments 



# TODO

- [] improve language 
- [] add AMPL version
- [] add instructions to build C++
- [] add bash script to conduct numerical experiments for heuristics
- [] add script to reproduce ampl experiments
