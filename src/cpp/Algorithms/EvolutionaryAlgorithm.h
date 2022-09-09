#ifndef NETWORKGENERATOR_EVOLUTIONARYALGORITHM_H
#define NETWORKGENERATOR_EVOLUTIONARYALGORITHM_H

#include "Bee.h"
#include "Country.h"

struct EvoMock {
  int m_;
};

class EvolutionaryAlgorithm {
private:
  Bee specimen;
  int generations;
  Network network;
  vector<Path> allPaths;

public:
  EvolutionaryAlgorithm(int id, const string &occupation, vector<Path> allPaths,
                        Network net);

  const Bee &getSpecimen() const;
  int getGenerations() const;
  const Network &getNetwork() const;

  void evolution(int abandonParameter, int op1, int op2, const Country &,
                 bool ifCalculateRating,
                 std::vector<int> evolutionaryAlgorithmTresholdValues);
  void divideRealisation();
  void printSpecimen();
};

#endif // NETWORKGENERATOR_EVOLUTIONARYALGORITHM_H
