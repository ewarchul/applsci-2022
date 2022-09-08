#include "BeeColony.h"
#include <fstream>

BeeColony::BeeColony(int workersNumber,
                     const vector<MyRealisation> &realisations,
                     const vector<Path> &allPaths, int rp, Network network1)
    : network(network1) {
  this->rejectionParameter = rp;
  this->allPaths = allPaths;
  int initialRealisationsNumber =
      network.getVertices().size() * (network.getVertices().size() - 1) / 2;
  for (int i = 0; i < workersNumber; i++)
    workers.emplace_back(i, "worker", realisations, allPaths, false,
                         initialRealisationsNumber);
}

// evaluates the quality of solutions of individual worker bees
void BeeColony::evaluateBees() {
  beeRatings.clear();

  double sum = 0.0;
  for (int i = 0; i < workers.size(); i++)
    sum += 1.0 / workers[i].getQuality();

  for (int i = 0; i < workers.size(); i++)
    beeRatings.push_back(1.0 / workers[i].getQuality() / sum);
}

// creates viewers and assigns them to specific workers
void BeeColony::sendOnlookers() {
  onlookers.clear();

  for (int i = 0; i < workers.size(); i++) {
    double left_sum = 0.0;
    double chosen_worker = rand() / double(RAND_MAX);
    int initialRealisationsNumber =
        network.getVertices().size() * (network.getVertices().size() - 1) / 2;
    for (int j = 0; j < beeRatings.size(); j++) {
      // if the worker bee has a better rating, there is a greater chance that a viewer will be assigned to her
      if (chosen_worker > left_sum && chosen_worker <= left_sum + beeRatings[j])
        onlookers.emplace_back(i, "onlooker", workers[j].getCars(),
                               workers[j].getCost(), j,
                               initialRealisationsNumber);

      left_sum += beeRatings[j];
    }
  }
}

// if a specific viewer has a better value than the worker to whom he is assigned, he passes his parameters to him
void BeeColony::replaceWorkers() {
  for (int i = 0; i < onlookers.size(); i++) {
    if (onlookers[i].getAssignedToWorker() == -1)
      cout << "\n\n Impossible to assign to worker with index smaller than "
              "zero!\n\n";

    if (onlookers[i].getCost() <
        workers[onlookers[i].getAssignedToWorker()].getCost()) {
      int workerID = workers[onlookers[i].getAssignedToWorker()].getId();
      workers[onlookers[i].getAssignedToWorker()] = onlookers[i];
      workers[onlookers[i].getAssignedToWorker()].setId(workerID);
      workers[onlookers[i].getAssignedToWorker()].setOccupation("worker");
      workers[onlookers[i].getAssignedToWorker()].setCyclesWithoutUpdate(0);
      workers[onlookers[i].getAssignedToWorker()].setAssignedToWorker(-1);
    }
  }
}

// each onlooker removes some of his worker's cars and draws new ones to replace them
void BeeColony::replaceCars(int coefficient) {
  coefficient = rand() % coefficient + 3;
  for (int i = 0; i < onlookers.size(); i++) {
    // the number of cars to be removed depends on the coefficient
    int carsToDelete = onlookers[i].getCars().size() / coefficient;
    vector<MyRealisation> freeRealisations;
    while (carsToDelete > 0) {
      int carIndex =
          rand() % onlookers[i]
                       .getCars()
                       .size(); // we draw the index of the car to be removed

      for (int j = 0;
           j < onlookers[i].getCars()[carIndex].getRealisationsTaken().size();
           j++) // we save realisations that will be left without a car
        freeRealisations.push_back(
            network.getRealisations()
                [onlookers[i].getCars()[carIndex].getRealisationsTaken()[j]]);

      onlookers[i].eraseCar(carIndex); // we remove the car
      carsToDelete--;
    }
    // we are trying to place the released realisations in already existing cars
    freeRealisations = onlookers[i].fillTheCars(freeRealisations);
    // we generate cars for uncontrolled realisations
    onlookers[i].generateCars(freeRealisations, this->allPaths);
  }
}

// calculates the cost for each bee in the colony
void BeeColony::calculateCosts(bool ifCalculateRating) {
  for (int i = 0; i < workers.size(); i++)
    workers[i].calculateCost(network.getRealisations().size(),
                             ifCalculateRating);

  for (int i = 0; i < onlookers.size(); i++)
    onlookers[i].calculateCost(network.getRealisations().size(),
                               ifCalculateRating);
}

void BeeColony::printColony() {
  cout << "\n Dane robotnikow: \n";
  for (int i = 0; i < workers.size(); i++)
    cout << "Worker, id: " << workers[i].getId()
         << ", cars: " << workers[i].getCars().size()
         << ", cost: " << workers[i].getCost() << endl;

  cout << "\n Dane widzow: \n";
  for (int i = 0; i < onlookers.size(); i++) {
    cout << "Onlooker, id: " << onlookers[i].getId()
         << ", assigned to worker: " << onlookers[i].getAssignedToWorker()
         << ", cars: " << onlookers[i].getCars().size()
         << ", cost: " << onlookers[i].getCost() << endl;
  }
}

// the main loop of the algorithm
int BeeColony::nectarHarvesting(int maxIterations, int op1Coefficient,
                                int op2Coefficient, const Country &country,
                                bool ifCalculateRating,
                                vector<int> tresholdValues) {
  std::clog << "Starting the ABC solver...\n";
  ofstream myFile;
  myFile.open("beeColonyLogs.csv", std::ios_base::app | std::ios_base::out);
  for (int i = 0; i < maxIterations; i++) {
    this->evaluateBees();  // we evaluate each worker
    this->sendOnlookers(); // we assign onlookers to workers

    int mode = rand() % 100;
    if (mode >= 0 && mode < 40)
      this->replaceCars(op1Coefficient); // we exchange cars at onlookers
    else if (mode >= 40 && mode < 90)
      this->switchRealisations(op2Coefficient); // at onlookers we mix realisations between cars
    else if (mode >= 90)
      this->mixCars(); // oonlookers randomly assign realisations to existing cars

    this->calculateCosts(ifCalculateRating);   // we calculate the cost of each solution
    this->replaceWorkers();   // we replace workers with their best onlooker (if any)
    this->findBestBeeIndex(); // we find the best solution

    this->incrementCycles(); // increments the cycle counters
    this->sendScouts(this->rejectionParameter); // we send scouts if the cycle condition is met

    if (i == tresholdValues[0]) {
      myFile << tresholdValues[0] << "\t" << workers[bestBeeIndex].getCost() << endl;
      tresholdValues.erase(tresholdValues.begin());
    }
  }

  workers[bestBeeIndex].removeReduntantArcs(network);
  workers[bestBeeIndex].checkCarTypes();
  workers[bestBeeIndex].calculateCost(network.getRealisations().size(),
                                      ifCalculateRating);

  // workers[bestBeeIndex].printBee(network);
  cout << "Ilosc pojazdow u najlepszego osobnika: "
       << workers[bestBeeIndex].getCars().size() << endl;

  myFile << "\n\n\n";
  myFile.close();
  return workers[bestBeeIndex].getRealCost();
}

// if the cycle condition is met, it sends out scouts
void BeeColony::sendScouts(int rp) {
  for (int i = 0; i < workers.size(); i++) {
    if (workers[i].getCyclesWithoutUpdate() >= rp && this->bestBeeIndex != i)
      workers[i].createScout(network, this->allPaths);
  }
}

int BeeColony::getRejectionParameter() const { return rejectionParameter; }

int BeeColony::getBestBeeIndex() const { return bestBeeIndex; }

// finds the index of the best bee
void BeeColony::findBestBeeIndex() {
  int minIndex = -1;
  int minValue = INT_MAX;

  for (int i = 0; i < workers.size(); i++) {
    if (workers[i].getCost() < minValue) {
      minValue = workers[i].getCost();
      minIndex = i;
    }
  }
  this->bestBeeIndex = minIndex;
}

// increments the cycle counter for all bees in the colony
void BeeColony::incrementCycles() {
  for (int i = 0; i < workers.size(); i++)
    workers[i].incrementCycles();
}

// we remove individual realisations from random cars, then we try to pack them to others,
// for those who failed we randomly select new cars
void BeeColony::switchRealisations(int coefficient) {
  coefficient = rand() % coefficient + 3;
  for (int i = 0; i < onlookers.size(); i++) {
    vector<MyRealisation> freeRealisations;
    int realisationsToRemove = onlookers[i].getCars().size() / coefficient;

    while (realisationsToRemove != 0) {
      // we draw a car and realisation to be removed
      int car = rand() % onlookers[i].getCars().size();
      int realisation =
          rand() % onlookers[i].getCars()[car].getRealisationsTaken().size();

      // we remember the deleted realizations
      freeRealisations.push_back(
          network.getRealisations()[onlookers[i]
                                        .getCars()[car]
                                        .getRealisationsTaken()[realisation]]);

      // we remove projects from a given car
      onlookers[i].removeRealisation(car, realisation, network);

      realisationsToRemove--;
    }
    // we assign projects to existing cars
    freeRealisations = onlookers[i].fillTheCars(freeRealisations);

    // we generate cars for other projects
    onlookers[i].generateCars(freeRealisations, this->allPaths);
  }
}

// operator reassigning realisations to existing cars
void BeeColony::mixCars() {
  for (int i = 0; i < onlookers.size(); i++) {
    // we unload cars
    onlookers[i].unloadCars();
    // we are trying to place the released projects in already existing cars
    onlookers[i].fillTheCarsBis(network.getRealisations(), this->allPaths);
  }
}

const vector<Bee> &BeeColony::getWorkers() const { return workers; }
