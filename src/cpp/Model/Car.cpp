#include "Car.h"

const vector<int> &Car::getMassCapacities() const { return massCapacities; }

const string &Car::getType() const { return type; }

const Path &Car::getPath() const { return path; }

const vector<int> &Car::getRealisationsTaken() const {
  return realisationsTaken;
}

int Car::getId() const { return id; }

int Car::getCapacity() const { return capacity; }

const vector<int> &Car::getVolumeCapacities() const { return volumeCapacities; }

// finds the start and end vertex indices of realisation in the path
vector<int> Car::findInterval(const MyRealisation &realisation) {
  vector<int> indexes;

  for (size_t i = 0; i < this->path.getVerticesVisited().size(); i++) {
    if (this->path.getVerticesVisited()[i] == realisation.getSource().getId())
      indexes.push_back(i);
    if (this->path.getVerticesVisited()[i] ==
        realisation.getDestination().getId())
      indexes.push_back(i);
  }

  return indexes;
}

void Car::loadRealisation(const MyRealisation &realisation) {
  realisationsTaken.push_back(realisation.getId());
  vector<int> indexes = this->findInterval(realisation);

  for (size_t i = indexes[0]; i < indexes[1]; i++) {
    this->massCapacities[i] -= realisation.getMass();
    this->volumeCapacities[i] -= realisation.getVolume();
  }
}

void Car::unloadRealisation(const MyRealisation &realisation) {
  for (size_t i = 0; i < this->realisationsTaken.size(); i++) {
    if (realisationsTaken[i] == realisation.getId()) {
      realisationsTaken.erase(realisationsTaken.begin() + i);
      break;
    }
  }

  vector<int> indexes = this->findInterval(realisation);

  for (size_t i = indexes[0]; i < indexes[1]; i++) {
    this->massCapacities[i] += realisation.getMass();
    this->volumeCapacities[i] += realisation.getVolume();
  }
}

Car::Car(int id, int type, Path path1) : path(path1) {
  this->id = id;

  if (type == 0) {
    this->type = "RENAULT";
    vector<int> initializer(path.getVerticesVisited().size() - 1, 25);
    this->massCapacities = initializer;
    this->volumeCapacities = initializer;
    this->capacity = 25;
  } else if (type == 1) {
    this->type = "FIAT";
    vector<int> initializer(path.getVerticesVisited().size() - 1, 50);
    this->massCapacities = initializer;
    this->volumeCapacities = initializer;
    this->capacity = 50;
  } else if (type == 2) {
    this->type = "TIR";
    vector<int> initializer(path.getVerticesVisited().size() - 1, 100);
    this->massCapacities = initializer;
    this->volumeCapacities = initializer;
    this->capacity = 100;
  }
}

// we check whether it is possible to shorten the car's route
void Car::cutPath(const Network &network) {
  int possibleStart = true;
  int possibleEnd = true;

  while (possibleStart == true || possibleEnd == true) {
    for (size_t i = 0; i < realisationsTaken.size(); i++) {
      // we check if it is possible to delete the beginning
      if (network.getRealisations()[realisationsTaken[i]].getSource().getId() ==
          this->path.getVerticesVisited()[0])
        possibleStart = false;

      // we check if it is possible to delete the end
      if (network.getRealisations()[realisationsTaken[i]]
              .getDestination()
              .getId() ==
          this->path.getVerticesVisited()[path.getVerticesVisited().size() - 1])
        possibleEnd = false;
    }

    if (possibleStart == true) {
      path.removeVertex(0, network.getVertices());
      massCapacities.erase(massCapacities.begin());
      volumeCapacities.erase(volumeCapacities.begin());
    }

    if (possibleEnd == true) {
      path.removeVertex(path.getVerticesVisited().size() - 1,
                        network.getVertices());
      massCapacities.erase(massCapacities.begin() + massCapacities.size() - 1);
      volumeCapacities.erase(volumeCapacities.begin() +
                             volumeCapacities.size() - 1);
    }
  }
}

void Car::divideRealisation(Network &network) {
  int realisationToDivide = rand() % this->realisationsTaken.size();
  int realisationIndex = this->realisationsTaken[realisationToDivide];
  MyRealisation realisationToRemove =
      network.getRealisations()[realisationIndex];
  vector<MyRealisation> newRealisations =
      network.divideRandomRealisation(realisationIndex);

  if (newRealisations.size() == 2) {
    this->unloadRealisation(realisationToRemove);
    this->loadRealisation(newRealisations[0]);
    this->loadRealisation(newRealisations[1]);
  }
}

// checks the possibility of loading the implementation into the car
bool Car::checkLoadingPossibility(const MyRealisation &realisation) {
  vector<int> indexes = this->findInterval(realisation);

  for (size_t i = indexes[0]; i < indexes[1]; i++) {
    if (this->massCapacities[i] - realisation.getMass() < 0 ||
        this->volumeCapacities[i] - realisation.getVolume() < 0)
      return false;
  }

  return true;
}

// removes all realisations from the car
void Car::unloadAllRealisations() {
  realisationsTaken.clear();

  if (this->type == "RENAULT") {
    vector<int> initializer(path.getVerticesVisited().size() - 1, 25);
    this->massCapacities = initializer;
    this->volumeCapacities = initializer;
  } else if (this->type == "FIAT") {
    vector<int> initializer(path.getVerticesVisited().size() - 1, 50);
    this->massCapacities = initializer;
    this->volumeCapacities = initializer;
  } else if (this->type == "TIR") {
    vector<int> initializer(path.getVerticesVisited().size() - 1, 100);
    this->massCapacities = initializer;
    this->volumeCapacities = initializer;
  }
}

// lists the parameters of the car
void Car::printCar(const Network &network) {
  cout << "Typ: " << type
       << ", liczba załadowanych realizacji: " << realisationsTaken.size()
       << " Przejechane kilometry: " << path.getLength() << endl;
  cout << "Poszczegolne pojemnosci: ";
  for (size_t i = 0; i < massCapacities.size(); i++)
    cout << massCapacities[i] << "-" << volumeCapacities[i] << ", ";

  cout << "\nZabrane realizacje: ";
  for (size_t i = 0; i < realisationsTaken.size(); i++)
    cout << realisationsTaken[i] << " - ";

  cout << "\nTrasa: ";
  for (size_t i = 0; i < path.getVerticesVisited().size(); i++)
    cout << network.getVertices()[path.getVerticesVisited()[i]].getName()
         << " - ";

  cout << "\n\n";
}

// checks if the car is empty enough to be changed
void Car::changeTypeOfCar() {
  if (this->type == "RENAULT")
    return;

  // if it is a TIR, we only check if it can be a Fiat, if so, the next if block will check it as a Fiat
  if (this->type == "TIR") {
    bool check = false;
    for (size_t i = 0; i < this->massCapacities.size(); i++) {
      if (this->massCapacities[i] < 50 || this->volumeCapacities[i] < 50) {
        check = true;
        break;
      }
    }
    if (!check) {
      this->type = "FIAT";
      for (size_t i = 0; i < this->massCapacities.size(); i++) {
        this->massCapacities[i] -= 50;
        this->volumeCapacities[i] -= 50;
      }
    }
  }

  // if there was a TIR, now it will also be checked as a Fiat whether it can become a Renault
  if (this->type == "FIAT") {
    bool check = false;
    for (size_t i = 0; i < this->massCapacities.size(); i++) {
      if (this->massCapacities[i] < 25 || this->volumeCapacities[i] < 25) {
        check = true;
        break;
      }
    }
    if (!check) {
      this->type = "RENAULT";
      for (size_t i = 0; i < this->massCapacities.size(); i++) {
        this->massCapacities[i] -= 25;
        this->volumeCapacities[i] -= 25;
      }
    }
  }
}
