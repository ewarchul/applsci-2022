#include "EvolutionaryAlgorithm.h"
#include <fstream>

EvolutionaryAlgorithm::EvolutionaryAlgorithm(int id, const string &occupation,
                                             vector<Path> allPaths,
                                             Network network1)
    : network(network1) {
  this->allPaths = allPaths;
  int initialRealisationsNumber =
      network.getVertices().size() * (network.getVertices().size() - 1) / 2;
  specimen = Bee(id, occupation, network.getRealisations(), allPaths, false,
                 initialRealisationsNumber);
  generations = 0;
}

// abandonParameter wskazuje po ilu iteracjach bez ulepszenia wyniku algorytm
// przestanie działać
void EvolutionaryAlgorithm::evolution(int abandonParameter, int o1, int o2,
                                      const Country &country,
                                      bool ifCalculateRating,
                                      vector<int> tresholdValues) {
  std::clog << "Starting the EA solver...\n";

  ofstream myFile;
  myFile.open("EALogs.csv", std::ios_base::app | std::ios_base::out);
  int iterations = 0;
  int abandonCounter = 0;
  while (abandonCounter < abandonParameter) {
    iterations++;
    if (iterations > 50000)
      break;

    int op1 = rand() % o1 + 3;
    int op2 = rand() % o2 + 3;

    Bee specimenBis = this->specimen;
    int mode = rand() % 100;

      // using a third operator who randomly reassigns realisations to existing cars, if a car becomes empty,
      // it is removed, and if realisations are not allocated, we generate new vehicles for them
    if (mode < 10) {
      // we unload cars
      specimenBis.unloadCars();
      // we are trying to place the released projects in already existing cars
      specimenBis.fillTheCarsBis(network.getRealisations(), this->allPaths);
    } else if (mode >= 10 && mode < 60) // realisation exchange operator
    {
      vector<MyRealisation> freeRealisations;
      int realisationsToRemove = specimenBis.getCars().size() / op1;

      while (realisationsToRemove != 0) {
        // we draw a car and realisation to be removed
        int car = rand() % specimenBis.getCars().size();
        int realisation =
            rand() % specimenBis.getCars()[car].getRealisationsTaken().size();

        // we remember the deleted realization
        freeRealisations.push_back(
            network
                .getRealisations()[specimenBis.getCars()[car]
                                       .getRealisationsTaken()[realisation]]);

        // we remove realisation from a given car
        specimenBis.removeRealisation(car, realisation, network);
        realisationsToRemove--;
      }

      // we assign realisations to existing cars
      freeRealisations = specimenBis.fillTheCars(freeRealisations);

      // we generate cars for other realisations
      specimenBis.generateCars(freeRealisations, this->allPaths);
    }
    else if (mode >= 60 && mode < 100) // car exchange operator
    {
      // the number of cars to be removed depends on the factor
      int carsToDelete = specimenBis.getCars().size() / op2;
      vector<MyRealisation> freeRealisations;
      while (carsToDelete > 0) {
        int carIndex =
            rand() % specimenBis.getCars()
                         .size(); // we draw the index of the car to be removed

        for (int j = 0;
             j < specimenBis.getCars()[carIndex].getRealisationsTaken().size();
             j++) // we save realisations that will be left without a car
          freeRealisations.push_back(
              network.getRealisations()[specimenBis.getCars()[carIndex]
                                            .getRealisationsTaken()[j]]);

        specimenBis.eraseCar(carIndex); // we remove the car
        carsToDelete--;
      }
      // we are trying to place the released realisations in already existing cars
      freeRealisations = specimenBis.fillTheCars(freeRealisations);
      // we generate cars for uncontrolled realisations
      specimenBis.generateCars(freeRealisations, this->allPaths);
    }

    specimenBis.calculateCost(network.getRealisations().size(),
                              ifCalculateRating);

    if (specimenBis.getCost() < specimen.getCost()) {
      specimen = specimenBis;
      abandonCounter = 0;
    } else
      abandonCounter++;

    generations++;

    if (iterations == tresholdValues[0]) {
      myFile << tresholdValues[0] << "\t" << specimen.getCost() << endl;
      tresholdValues.erase(tresholdValues.begin());
    }
  }
  specimen.removeReduntantArcs(network);
  specimen.checkCarTypes();
  specimen.calculateCost(network.getRealisations().size(), ifCalculateRating);
  myFile << "\n\n\n";
  myFile.close();
}

void EvolutionaryAlgorithm::divideRealisation() {}

const Bee &EvolutionaryAlgorithm::getSpecimen() const { return specimen; }

int EvolutionaryAlgorithm::getGenerations() const { return generations; }

void EvolutionaryAlgorithm::printSpecimen() {
  specimen.printBee(this->network);
}

const Network &EvolutionaryAlgorithm::getNetwork() const { return network; }
