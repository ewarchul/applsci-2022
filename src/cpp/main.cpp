#include "Algorithms/Algorithms.h"
#include "Model/Model.h"
#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>

constexpr int max_path_lengths{15};

/*
 * Generate DAT file for AMPL
 *
 * This function generates a .dat file for the program in the AMPL language.
 *
 */

void generate_dat(Network &network, const Country &country) {
  if (Network::check_crossroads(country.getCities()[0], country.getCities()[5],
                                country.getCities()[5], country.getCities()[0]))
    std::cout << "Przecinaja sie" << std::endl;
  else
    std::cout << "Nie przecinaja sie" << std::endl;

  srand(time(nullptr));

  std::cout << network.getVertices().size() << std::endl;
  std::cout << network.getArcs().size() << std::endl;
  for (size_t i = 0; i < network.getVertices().size(); i++) {
    std::cout << "vertex " << network.getVertices()[i].getName() << " arcsl: ";
    for (size_t j = 0; j < network.getVertices()[i].getArcsLeaving().size();
         j++) {
      std::cout << network.getVertices()[i].getArcsLeaving()[j] << ", ";
    }
    std::cout << "; \n";

    std::cout << "vertex " << network.getVertices()[i].getName() << " arcse: ";
    for (size_t j = 0; j < network.getVertices()[i].getArcsEntering().size();
         j++) {
      std::cout << network.getVertices()[i].getArcsEntering()[j] << ", ";
    }
    std::cout << "; \n";
  }

  for (size_t i = 0; i < network.getArcs().size(); i++) {
    std::cout << "arc " << i
              << " source: " << network.getArcs()[i].getSource().getName()
              << std::endl;
    std::cout << "arc " << i
              << " destination: " << network.getArcs()[i].getTarget().getName()
              << std::endl;
  }

  Writer writer;
  writer.createExtendedDat(network);
  network.divideRealisations();
  std::cout << std::endl;
}

std::vector<Path> prepare_paths(const Country &country,
                                const Network &network) {
  DFS dfs(network.getVertices(), network.getArcs(),
          network.getVertices().size());
  vector<Path> tmp_paths;
  vector<Path> paths;
  for (size_t i = 0; i < network.getVertices().size() - 1; i++) {
    for (size_t j = i + 1; j < network.getVertices().size(); j++) {
      dfs.findpaths(network.getVertices()[i].getId(),
                    network.getVertices()[j].getId());
      tmp_paths = dfs.pickSolutions(3, country);

      for (size_t k = 0; k < tmp_paths.size(); k++) {
        if (tmp_paths[k].getVerticesVisited().size() <= max_path_lengths)
          paths.push_back(tmp_paths[k]);
      }

      tmp_paths.clear();
      dfs.clearSolutions();
    }
    std::cout << "paths for city number " << i << " done" << std::endl;
  }

  return paths;
}

/*
 * Run the ABC solver
 */

void run_abc(const Country &country, const Network &network,
             std::vector<Path> all_paths) {
  std::vector<int> beeColonyTresholdValues = {0,  3,   7,   17,  33,
                                              67, 167, 333, 667, 1699};
  std::vector<int> beeColoniesResults;

  std::cout << "Population 30, rp 40, iterations 1700, coefficients 3-12\n";

  int bestResult = INT_MAX;
  int avg = 0;
  auto begin = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < 20; i++) {
    auto time_tmp_start = std::chrono::high_resolution_clock::now();

    BeeColony beeColony(30, network.getRealisations(), all_paths, 40, network);
    int beeColonyResult = beeColony.nectarHarvesting(
        1700, 10, 10, country, false, beeColonyTresholdValues);

    auto time_tmp_end = std::chrono::high_resolution_clock::now();

    beeColoniesResults.push_back(beeColonyResult);

    auto tmp_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_tmp_end - time_tmp_start);

    if (beeColonyResult < bestResult)
      bestResult = beeColonyResult;

    std::cout << "BeeColony " << i << " o wartosci " << beeColonyResult
              << " ukonczone. Ostatnio zmodyfikowane: "
              << beeColony.getWorkers()[beeColony.getBestBeeIndex()]
                     .getCyclesWithoutUpdate();
    printf(". Czas BeeColony: %.3f sekund.\n", tmp_elapsed.count() * 1e-9);
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
  printf("Czas sumaryczny BeeColonies: %.3f sekund.\n", elapsed.count() * 1e-9);

  std::cout << "Najlepszy rezultat: " << std::endl;
  std::cout << bestResult << std::endl;

  for (size_t i = 0; i < beeColoniesResults.size(); i++) {
    avg += beeColoniesResults[i];
  }

  std::cout << "Sredni rezultat BeeColonies: " << std::endl;
  std::cout << avg / beeColoniesResults.size() << std::endl;
}

/*
 * Run the EA solver
 */

void run_ea(const Country &country, const Network &network,
            const std::vector<Path> &all_paths) {
  std::vector<int> evolutionaryAlgorithmTresholdValues = {
      1, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 49999};
  std::vector<int> EAResults;
  int bestResultEA = INT_MAX;
  int avgEA = 0;

  auto beginBis = std::chrono::high_resolution_clock::now();
  for (size_t i = 0; i < 20; i++) {
    auto time_tmp_start = std::chrono::high_resolution_clock::now();

    EvolutionaryAlgorithm evolutionaryAlgorithm(1, "DarwinBis", all_paths,
                                                network);

    evolutionaryAlgorithm.evolution(10000, 10, 10, country, false,
                                    evolutionaryAlgorithmTresholdValues);

    auto time_tmp_end = std::chrono::high_resolution_clock::now();

    std::cout << "Koszt: " << evolutionaryAlgorithm.getSpecimen().getCost()
              << " po " << evolutionaryAlgorithm.getGenerations()
              << " generacjach. Siec ma rozmiar: "
              << evolutionaryAlgorithm.getNetwork().getRealisations().size();

    auto tmp_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(
        time_tmp_end - time_tmp_start);

    printf(". Czas : %.3f sekund.\n", tmp_elapsed.count() * 1e-9);

    int resultEA = evolutionaryAlgorithm.getSpecimen().getRealCost();
    EAResults.push_back(resultEA);
    if (resultEA < bestResultEA) {
      bestResultEA = resultEA;
    }
  }

  auto endBis = std::chrono::high_resolution_clock::now();
  auto elapsedBis =
      std::chrono::duration_cast<std::chrono::nanoseconds>(endBis - beginBis);
  printf("Czas sumaryczny AE: %.3f sekund.\n", elapsedBis.count() * 1e-9);

  std::cout << "Najlepszy rezultat: " << std::endl;
  std::cout << bestResultEA << std::endl;

  for (size_t i = 0; i < EAResults.size(); i++) {
    avgEA += EAResults[i];
  }
  std::cout << "Sredni rezultat EAs: " << std::endl;
  std::cout << avgEA / EAResults.size() << std::endl;
}

bool cmdopt_exists(char **begin, char **end, const std::string &opt) {
  return std::find(begin, end, opt) != end;
}

void usage() {
  std::cout << "Usage: ./NetworkSolver [--generate-dat] --method=[ABC|EA]\n";
  exit(1);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    usage();
  }

  constexpr std::array<double, 4> coords{55.0, 50.25, 10.0, 40.0};
  const auto &[north, south, west, east] = coords;
  Country country;
  Network network(country, north, south, west, east);

  auto all_paths = prepare_paths(country, network);

  if (cmdopt_exists(argv, argv + argc, "--generate-dat")) {
    generate_dat(network, country);
  }

  if (cmdopt_exists(argv, argv + argc, "--method=ABC")) {
    run_abc(country, network, all_paths);
  } else if (cmdopt_exists(argv, argv + argc, "--method=EA")) {
    run_ea(country, network, all_paths);
  } else {
    usage();
  }

  return 0;
}

// ./NetworkSolver [--generate-dat] --method EA
