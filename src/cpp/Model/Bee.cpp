#include "Bee.h"

// Worker's constructor
Bee::Bee(int id, const string &occupation,
         const vector<MyRealisation> &realisations, vector<Path> allPaths,
         bool ifCalculateRating, int initialRealisationsNumber) {
  this->id = id;
  this->occupation = occupation;
  this->initialRealisationsNumber = initialRealisationsNumber;

  // cars of given specimen are generated, they will carry all realisations
  generateCars(realisations, std::move(allPaths));
  // we calculate the cost of given specimen
  calculateCost(realisations.size(), ifCalculateRating);
}

// Onlooker's constructor
Bee::Bee(int id, const string &occupation, const vector<Car> &cars, int cost,
         int assignedToWorker, int initialRealisationsNumber) {
  this->id = id;
  this->occupation = occupation;
  this->cars = cars;
  this->cost = cost;
  this->assignedToWorker = assignedToWorker;
  this->initialRealisationsNumber = initialRealisationsNumber;
}

int Bee::getId() const { return id; }

int Bee::getAssignedToWorker() const { return assignedToWorker; }

const string &Bee::getOccupation() const { return occupation; }

const vector<Car> &Bee::getCars() const { return cars; }

void Bee::setCyclesWithoutUpdate(int cyclesWithoutUpdate1) {
  Bee::cyclesWithoutUpdate = cyclesWithoutUpdate1;
}

void Bee::incrementCycles() { this->cyclesWithoutUpdate++; }

void Bee::eraseCar(int index) { this->cars.erase(cars.begin() + index); }

void Bee::setId(int id) { Bee::id = id; }

void Bee::setOccupation(const string &occupation) {
  Bee::occupation = occupation;
}

int Bee::getCyclesWithoutUpdate() const { return cyclesWithoutUpdate; }

int Bee::getCost() const { return cost; }

void Bee::setCost(int cost) { Bee::cost = cost; }

int Bee::getRealCost() const { return realCost; }

int Bee::getQuality() const { return quality; }

void Bee::calculateCost(int realisationsNumber, bool ifCalculateRating) {
  int costBis = 0;
  for (int i = 0; i < cars.size(); i++) {
    int tmp =
        cars[i].getPath().getLength();

    if (cars[i].getType() == "RENAULT")
      tmp /= 10; // 25
    else if (cars[i].getType() == "FIAT")
      tmp /= 6.66; // 50
    else if (cars[i].getType() == "TIR")
      tmp /= 3.33; // 100
    costBis += tmp;
  }

  costBis += initialRealisationsNumber * 20;
  this->cost = costBis;
  this->realCost = costBis;
  if (ifCalculateRating)
    this->calculateRating();
  this->quality =
      this->cost * this->cost *
      this->cost;
}

// calculates an individual's score on the basis of the average vehicle load
void Bee::calculateRating() {
  int totalCapacity = 0;
  for (int i = 0; i < cars.size(); i++)
    totalCapacity += cars[i].getCapacity();
  int averageCapacity = totalCapacity / cars.size();
  double coefficient = (double)cost / 500;
  this->cost = static_cast<int>(this->cost + (averageCapacity * coefficient));
}

void Bee::setAssignedToWorker(int worker) { this->assignedToWorker = worker; }

// creates cars as long as all projects are allocated
void Bee::generateCars(
    const vector<MyRealisation> &realisations,
    vector<Path> allPaths)
{
  int unsubjectedRealisations = realisations.size();
  int carCounter = this->cars.size();
  vector<MyRealisation> tmpRealisations = realisations;

  while (unsubjectedRealisations != 0) {
    int type = rand() % VEHICLES_NUMBER;
    int pathIndex = rand() % allPaths.size();
    Path path = allPaths[pathIndex];
    Car car(carCounter, type, path);

    int checkInclusionResult = checkInclusion(path, tmpRealisations);
    while (checkInclusionResult != -1) {
      // we check if it is possible to load the projects, if not then we are not looking for this car any longer
      if (!car.checkLoadingPossibility(tmpRealisations[checkInclusionResult]))
        break;

      car.loadRealisation(tmpRealisations[checkInclusionResult]);
      // we remove already assigned realisation
      tmpRealisations.erase(tmpRealisations.begin() + checkInclusionResult);
      unsubjectedRealisations--;

      // we are trying to load another realisation
      checkInclusionResult = checkInclusion(path, tmpRealisations);
    }

    if (!car.getRealisationsTaken().empty()) {
      carCounter++;
      cars.push_back(car);
      // cout << "Car number " << carCounter << " added.\n" <<
      // unsubjectedRealisations << " unsubjected realisations left.\n";
    }
  }
}

// checks if there is any realization in the given path
int Bee::checkInclusion(const Path &path, vector<MyRealisation> realisations) {
  // vector of realisations included in the path of the car
  vector<int> allInclRealisations;

  for (int i = 0; i < realisations.size(); i++) {
    bool startFound = false;
    for (int j = 0; j < path.getVerticesVisited().size(); j++) {
      if (realisations[i].getSource().getId() == path.getVerticesVisited()[j])
        startFound = true;

      if (startFound &&
          realisations[i].getDestination().getId() ==
              path.getVerticesVisited()[j]) // we return the index of the realisation contained in the path given in the argument
        allInclRealisations.push_back(i);
    }
  }

  if (!allInclRealisations.empty()) {
    int randomIndex = rand() % allInclRealisations.size();
    return allInclRealisations[randomIndex];
  }
  return -1;
}

// turns the bee into a scout
void Bee::createScout(const Network &network, vector<Path> allPaths) {
  this->cars.clear();
  this->cost = 0;
  this->cyclesWithoutUpdate = 0;
  this->assignedToWorker = -1;

  this->generateCars(network.getRealisations(), allPaths);
}

void Bee::printBee(const Network &network) {
  cout << "Printing bee number " << this->id << " with cost " << this->cost
       << endl;
  for (int i = 0; i < cars.size(); i++)
    this->cars[i].printCar(network);
}

// We are trying to place realisations from previously removed cars in existing ones
vector<MyRealisation> Bee::fillTheCars(vector<MyRealisation> realisations) {
  vector<int> carIndexes;
  carIndexes.reserve(cars.size());

  for (int i = 0; i < cars.size(); i++)
    carIndexes.push_back(i);

  while (!carIndexes.empty()) {
    int tmp = rand() % carIndexes.size();
    int i = carIndexes[tmp];

    int checkInclusionResult = checkInclusion(cars[i].getPath(), realisations);
    while (checkInclusionResult != -1) {
      // we check if it is possible to load the realisations, if not then we are not looking for this car any longer
      if (!cars[i].checkLoadingPossibility(realisations[checkInclusionResult]))
        break;

      cars[i].loadRealisation(realisations[checkInclusionResult]);
      // we remove already assigned realisations
      realisations.erase(realisations.begin() + checkInclusionResult);

      // we are trying to load another realisation
      checkInclusionResult = checkInclusion(cars[i].getPath(), realisations);
    }
    carIndexes.erase(carIndexes.begin() + tmp);
  }
  return realisations;
}

// removes realization from a given car
void Bee::removeRealisation(int carIndex, int realisationIndex,
                            const Network &network) {
  this->cars[carIndex].unloadRealisation(
      network.getRealisations()[cars[carIndex]
                                    .getRealisationsTaken()[realisationIndex]]);

  if (this->cars[carIndex].getRealisationsTaken().empty()) {
    this->cars.erase(this->cars.begin() + carIndex);
    // cout << "usunieto samochod" << endl;
  }
}

// each car checks if it can shorten its routes
void Bee::removeReduntantArcs(const Network &network) {
  for (int i = 0; i < this->cars.size(); i++)
    cars[i].cutPath(network);
}

void Bee::unloadCars() {
  for (int i = 0; i < this->cars.size(); i++)
    cars[i].unloadAllRealisations();
}

// the passed realisations parameter has all the realisations of the network
void Bee::fillTheCarsBis(const vector<MyRealisation> &real,
                         const vector<Path> &allPaths) {
  vector<MyRealisation> realisations = real;
  int realisationIndex = rand() % realisations.size();
  int counter = realisations.size();

  vector<int> carsTmp;
  carsTmp.reserve(this->cars.size());
  for (int i = 0; i < this->cars.size(); i++)
    carsTmp.push_back(i);

  while (counter != 0) {
    bool resultBis = false;
    int carIndex = rand() % carsTmp.size();
    vector<MyRealisation> singleRealisation;
    singleRealisation.push_back(realisations[realisationIndex]);

    // we pass a single-element vector because checkInclusion uses vectors
    int result = checkInclusion(this->cars[carsTmp[carIndex]].getPath(),
                                singleRealisation);

    // if the route of realisation is inside the car, we check if it can be loaded into it
    if (result != -1)
      resultBis = this->cars[carsTmp[carIndex]].checkLoadingPossibility(
          singleRealisation[0]);

    if (resultBis) {
      this->cars[carsTmp[carIndex]].loadRealisation(singleRealisation[0]);
      realisations.erase(realisations.begin() + realisationIndex);

      if (realisations.empty())
        break;

      realisationIndex = rand() % realisations.size();
      counter--;
      carsTmp.clear();
      for (int i = 0; i < this->cars.size(); i++)
        carsTmp.push_back(i);
    } else
      carsTmp.erase(carsTmp.begin() + carIndex);

    // if we could not find a car for a given realisation, create a new one and add it to the list of cars
    if (carsTmp.empty()) {
      this->generateCars(singleRealisation, allPaths);
      realisations.erase(realisations.begin() + realisationIndex);
      if (realisations.empty())
        break;
      realisationIndex = rand() % realisations.size();
      carsTmp.clear();
      for (int i = 0; i < this->cars.size(); i++)
        carsTmp.push_back(i);
    }
  }

  // we remove cars that have been empty
  for (int i = 0; i < this->cars.size(); i++) {
    if (this->cars[i].getRealisationsTaken().empty()) {
      cars.erase(cars.begin() + i);
      i--;
    }
  }
}

Bee::Bee() {}

// checks each car for a change of its type
void Bee::checkCarTypes() {
  for (int i = 0; i < this->cars.size(); i++)
    this->cars[i].changeTypeOfCar();
}

void Bee::divideRealisation(int carIndex, Network &network) {
  this->cars[carIndex].divideRealisation(network);
}
