#include "EvolutionaryAlgorithm.h"
#include <fstream>

EvolutionaryAlgorithm::EvolutionaryAlgorithm(int id, const string &occupation, vector<Path> allPaths, Network network1) : network(network1)
{
    this->allPaths = allPaths;
    int initialRealisationsNumber =  network.getVertices().size() * (network.getVertices().size() - 1) / 2;
    specimen = Bee(id, occupation, network.getRealisations(), allPaths, false, initialRealisationsNumber);
    generations = 0;
}

// abandonParameter wskazuje po ilu iteracjach bez ulepszenia wyniku algorytm przestanie działać
void EvolutionaryAlgorithm::evolution(int abandonParameter, int o1, int o2, const Country& country, bool ifCalculateRating, vector <int> tresholdValues)
{
    ofstream myFile;
    myFile.open("EALogs.csv", std::ios_base::app | std::ios_base::out);
    int iterations = 0;
    int abandonCounter = 0;
    while (abandonCounter < abandonParameter)
    {
        iterations++;
        if (iterations > 50000)
            break;

        int op1 = rand() % o1 + 3;
        int op2 = rand() % o2 + 3;

        Bee specimenBis = this->specimen;
        Network networkBis = this->network;
        int mode = rand() % 10;

        // użycie operatora trzeciego, który losowo przydziela ponownie realizacje do istniejacych samochodow
        // jesli jakis samochod stanie sie pusty to jest usuwany, a jesli zostana nieprzydzielone realizacje
        // to generujemy dla nich nowe pojazdy
        if (mode == 0 )
        {
            // rozładowujemy samochody
            specimenBis.unloadCars();
            // probujemy umiescic zwolnione realizacje w juz istniejacych samochodach
            specimenBis.fillTheCarsBis(network.getRealisations(), this->allPaths);
        }
        else if (mode == 1 || mode == 2 || mode == 3 || mode == 4 || mode == 5)     // operator wymiany realizacji
        {
            vector<MyRealisation> freeRealisations;
            int realisationsToRemove = specimenBis.getCars().size()/op1;

            while (realisationsToRemove != 0)
            {
                // losujemy samochod i realizacje do usuniecia
                int car = rand() % specimenBis.getCars().size();
                int realisation = rand() % specimenBis.getCars()[car].getRealisationsTaken().size();

                // zapamietujemy usunieta realizacje
                freeRealisations.push_back(network.getRealisations()[specimenBis.getCars()[car].getRealisationsTaken()[realisation]]);

                // usuwamy realizacje z danego samochodu
                specimenBis.removeRealisation(car, realisation, network);
                realisationsToRemove--;
            }

            //przydzielamy realizacje istniejacym samochodom
            freeRealisations = specimenBis.fillTheCars(freeRealisations);

            // generujemy samochody dla pozostalych realizacji
            specimenBis.generateCars(freeRealisations, this->allPaths);
        }
//        else if (mode == 5)     // operator przesiadania
//        {
//            int carIndex = rand() % specimenBis.getCars().size();
//            specimenBis.divideRealisation(carIndex, networkBis);
//        }
        else if (mode == 6 ||mode == 7 || mode == 8 || mode == 9)     // operator wymiany samochodow
        {
            // ilosc samochodow do usuniecia zalezy od wspolczynnika
            int carsToDelete = specimenBis.getCars().size() / op2;
            vector<MyRealisation> freeRealisations;
            while (carsToDelete > 0)
            {
                int carIndex = rand() % specimenBis.getCars().size();        // losujemy indeks samochodu do usuniecia

                for (int j = 0; j < specimenBis.getCars()[carIndex].getRealisationsTaken().size(); j++)        // zapisujemy realizacje, które zostaną bez samochodu
                    freeRealisations.push_back(network.getRealisations()[specimenBis.getCars()[carIndex].getRealisationsTaken()[j]]);

                specimenBis.eraseCar(carIndex);         // usuwamy samochod
                carsToDelete--;
            }
            // probujemy umiescic zwolnione realizacje w juz istniejacych samochodach
            freeRealisations = specimenBis.fillTheCars(freeRealisations);
            // generujemy samochody dla niepodporzadkowanych realizacji
            specimenBis.generateCars(freeRealisations, this->allPaths);
        }

        specimenBis.calculateCost(network.getRealisations().size(), ifCalculateRating);
        // losujemy osobnika ktory przejdzie dalej
        int chosenSpecimen = rand() % 2;
        if (specimenBis.getCost() < specimen.getCost())
        {
            specimen = specimenBis;
            abandonCounter = 0;
            // jesli operator mutujący sieć polepszył rozwiązanie to przed następną iteracją należy podmienić sieci
            this->network = networkBis;
        }
        else
            abandonCounter++;

        generations++;

        if (iterations == tresholdValues[0])
        {
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

void EvolutionaryAlgorithm::divideRealisation()
{

}

const Bee &EvolutionaryAlgorithm::getSpecimen() const {
    return specimen;
}

int EvolutionaryAlgorithm::getGenerations() const {
    return generations;
}

void EvolutionaryAlgorithm::printSpecimen()
{
    specimen.printBee(this->network);
}

const Network &EvolutionaryAlgorithm::getNetwork() const {
    return network;
}