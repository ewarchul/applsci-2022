#ifndef NETWORKGENERATOR_BEECOLONY_H
#define NETWORKGENERATOR_BEECOLONY_H

#include <vector>
#include <iostream>
#include <utility>
#include <climits>
#include "Bee.h"

class BeeColony {
private:
    vector<Path> allPaths;
    vector<Bee> workers;
    vector<Bee> onlookers;
    std::vector<double> beeRatings;  // i-ty realCost odpowiada i-tej pszczole
    int rejectionParameter;
    int bestBeeIndex;
    Network network;

public:
    BeeColony(int workersNumber, const vector<MyRealisation> &realisations, const vector<Path> &allPaths,
              int rp, Network network1);

    int getRejectionParameter() const;
    int getBestBeeIndex() const;
    const vector<Bee> &getWorkers() const;

    void evaluateBees();
    void sendOnlookers();
    void replaceWorkers();
    void calculateCosts(bool ifCalculateRating);
    void printColony();
    int nectarHarvesting(int it, int op1Coefficient, int op2Coefficient, const Country &country, bool ifCalculateRating, vector<int> tresholdValues);
    void sendScouts(int rp);
    void findBestBeeIndex();
    void incrementCycles();
    void replaceCars(int coefficient);
    void modifyRealisations(int minL, int maxL, int coefficient);
    void switchRealisations(int coefficient);
    void mixCars();
};


#endif //NETWORKGENERATOR_BEECOLONY_H
