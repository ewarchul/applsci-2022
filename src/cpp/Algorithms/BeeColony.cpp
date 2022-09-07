#include "BeeColony.h"
#include <fstream>


BeeColony::BeeColony(int workersNumber, const vector<MyRealisation> &realisations, const vector<Path> &allPaths, int rp, Network network1) : network(network1)
{
    this->rejectionParameter = rp;
    this->allPaths = allPaths;
    int initialRealisationsNumber =  network.getVertices().size() * (network.getVertices().size() - 1) / 2;
    for (int i = 0; i < workersNumber; i++)
        workers.emplace_back(i, "worker", realisations, allPaths, false, initialRealisationsNumber);
}

//ocenia jakosc rozwiazan poszczegolnych pszczol robotnic
void BeeColony::evaluateBees()
{
    beeRatings.clear();

    double sum = 0.0;
    for (int i = 0; i < workers.size(); i++)
        sum += 1.0/workers[i].getQuality();

    for (int i = 0; i < workers.size(); i++)
        beeRatings.push_back(1.0/workers[i].getQuality()/sum);
}

//tworzy widzow oraz przypisuje ich do okreslonych robotnikow
void BeeColony::sendOnlookers()
{
    onlookers.clear();

    for (int i = 0; i < workers.size(); i++)
    {
        double left_sum = 0.0;
        double chosen_worker = rand() / double(RAND_MAX);
        int initialRealisationsNumber =  network.getVertices().size() * (network.getVertices().size() - 1) / 2;
        for (int j = 0; j < beeRatings.size(); j++)
        {
            // jesli pszczola robotnica ma lepsza ocene to istnieje wieksza szansa, ze widz trafi do jej przedzialu
            if (chosen_worker > left_sum && chosen_worker <= left_sum + beeRatings[j])
                onlookers.emplace_back(i, "onlooker", workers[j].getCars(), workers[j].getCost(), j, initialRealisationsNumber);

            left_sum += beeRatings[j];
        }
    }
}

// jesli okreslony widz ma lepsza wartosc niz robotnik, do ktorego jest przypisany to przekazuje mu swoje parametry
void BeeColony::replaceWorkers()
{
    for (int i = 0; i < onlookers.size(); i++)
    {
        if (onlookers[i].getAssignedToWorker() == -1)
            cout << "\n\n Impossible to assign to worker with index smaller than zero!\n\n";

        if (onlookers[i].getCost() < workers[onlookers[i].getAssignedToWorker()].getCost())
        {
            int workerID = workers[onlookers[i].getAssignedToWorker()].getId();
            workers[onlookers[i].getAssignedToWorker()] = onlookers[i];
            workers[onlookers[i].getAssignedToWorker()].setId(workerID);
            workers[onlookers[i].getAssignedToWorker()].setOccupation("worker");
            workers[onlookers[i].getAssignedToWorker()].setCyclesWithoutUpdate(0);
            workers[onlookers[i].getAssignedToWorker()].setAssignedToWorker(-1);
        }
    }
}

// kazdy onlooker usuwa czesc samochodow swojego workera i losuje nowe na ich miejsce
void BeeColony::replaceCars(int coefficient)
{
    coefficient = rand() % coefficient + 3;
    for (int i = 0; i < onlookers.size(); i++)
    {
        // ilosc samochodow do usuniecia zalezy od wspolczynnika
        int carsToDelete = onlookers[i].getCars().size() / coefficient;
        vector<MyRealisation> freeRealisations;
        while (carsToDelete > 0)
        {
            int carIndex = rand()%onlookers[i].getCars().size();        // losujemy indeks samochodu do usuniecia

            for (int j = 0; j < onlookers[i].getCars()[carIndex].getRealisationsTaken().size(); j++)        // zapisujemy realizacje, które zostaną bez samochodu
                freeRealisations.push_back(network.getRealisations()[onlookers[i].getCars()[carIndex].getRealisationsTaken()[j]]);

            onlookers[i].eraseCar(carIndex);         // usuwamy samochod
            carsToDelete--;
        }
        // probujemy umiescic zwolnione realizacje w juz istniejacych samochodach
        freeRealisations = onlookers[i].fillTheCars(freeRealisations);
        // generujemy samochody dla niepodporzadkowanych realizacji
        onlookers[i].generateCars(freeRealisations, this->allPaths);
    }
}

// oblicza koszt dla kazdej pszczoly w kolonii
void BeeColony::calculateCosts(bool ifCalculateRating)
{
    for (int i = 0; i < workers.size(); i++)
        workers[i].calculateCost(network.getRealisations().size(), ifCalculateRating);

    for (int i = 0; i < onlookers.size(); i++)
        onlookers[i].calculateCost(network.getRealisations().size(), ifCalculateRating);
}

void BeeColony::printColony()
{
    cout << "\n Dane robotnikow: \n";
    for (int i = 0; i < workers.size(); i++)
        cout << "Worker, id: " << workers[i].getId() << ", cars: " << workers[i].getCars().size() << ", cost: " << workers[i].getCost() << endl;

    cout << "\n Dane widzow: \n";
    for (int i = 0; i < onlookers.size(); i++)
    {
        cout << "Onlooker, id: " << onlookers[i].getId() << ", assigned to worker: " << onlookers[i].getAssignedToWorker()
             <<  ", cars: " << onlookers[i].getCars().size() << ", cost: " << onlookers[i].getCost() << endl;
    }
}

// glowna petla algorytmu
int BeeColony::nectarHarvesting(int maxIterations, int op1Coefficient, int op2Coefficient, const Country& country, bool ifCalculateRating, vector<int> tresholdValues)
{
    ofstream myFile;
    myFile.open("beeColonyLogs.csv", std::ios_base::app | std::ios_base::out);
    for (int i = 0; i < maxIterations; i++)
    {
        this->evaluateBees();       // oceniamy kazdego workera
        this->sendOnlookers();      // przydzielamy onlookerow do workerow

        int mode = rand() % 10;
        if (mode == 0 || mode == 1 || mode == 2 || mode == 3)
            this->replaceCars(op1Coefficient);      // u onlookerow wymieniamy samochody
        else if (mode == 4 || mode == 5 || mode == 6 || mode == 7 || mode == 8)
            this->switchRealisations(op2Coefficient);   // u onlookerow mieszamy realizacje miedzy samochodami
        else if (mode == 9)
            this->mixCars();   // onlookerzy losowo przydzielają realizacje do istniejacych samochodow

        this->calculateCosts(ifCalculateRating);     // obliczamy koszty kazdego rozwiazania
        this->replaceWorkers();     // zastepujemy workerow ich najlepszym onlookerem (jesli istnieje)
        this->findBestBeeIndex();   // odnajdujemy najlepsze rozwiazanie

        this->incrementCycles();    // inkrementujemy liczniki cykli bez zastapienia
        this->sendScouts(this->rejectionParameter);    // wysylamy zwiadowcow, jesli spelniony zostal warunek cykli

        if (i == tresholdValues[0])
        {
            myFile << tresholdValues[0] << "\t" << workers[bestBeeIndex].getCost() << endl;
            tresholdValues.erase(tresholdValues.begin());
        }
    }

    workers[bestBeeIndex].removeReduntantArcs(network);
    workers[bestBeeIndex].checkCarTypes();
    workers[bestBeeIndex].calculateCost(network.getRealisations().size(), ifCalculateRating);

    //workers[bestBeeIndex].printBee(network);
    cout << "Ilosc pojazdow u najlepszego osobnika: " << workers[bestBeeIndex].getCars().size() << endl;

    myFile << "\n\n\n";
    myFile.close();
    return workers[bestBeeIndex].getRealCost();
}

// jesli spelniony jest warunek cykli to wysyla zwiadowcow
void BeeColony::sendScouts(int rp)
{
    for (int i = 0; i < workers.size(); i++)
    {
        if (workers[i].getCyclesWithoutUpdate() >= rp && this->bestBeeIndex != i)
            workers[i].createScout(network, this->allPaths);
    }
}

int BeeColony::getRejectionParameter() const {
    return rejectionParameter;
}

int BeeColony::getBestBeeIndex() const {
    return bestBeeIndex;
}

// znajduje indeks najlepszej pszczoly
void BeeColony::findBestBeeIndex()
{
    int minIndex = -1;
    int minValue = INT_MAX;

    for (int i = 0; i < workers.size(); i++)
    {
        if (workers[i].getCost() < minValue)
        {
            minValue = workers[i].getCost();
            minIndex = i;
        }
    }
    this->bestBeeIndex = minIndex;
}

// inkrementuje licznik cykli wszystkim pszczolom w kolonii
void BeeColony::incrementCycles()
{
    for (int i = 0; i < workers.size(); i++)
        workers[i].incrementCycles();
}

// losuje realiacje sposrod juz przydzielonych, a nastepnie dzieli je lub laczy z innymi, spelniajac warunki minimalnej
// oraz maksymalnej dlugosci // todo skoncz
void BeeColony::modifyRealisations(int minL, int maxL, int coefficient)
{
    for (int i = 0; i < onlookers.size(); i++)
    {
        int realisationsToModify = onlookers[i].getCars().size() / coefficient;
        while(realisationsToModify != 0)
        {
            int car = rand()%onlookers[i].getCars().size();     // samochod z ktorego wylosujemy realizacje do zmodyfikowania
            for (int j = 0; j < onlookers[i].getCars()[car].getRealisationsTaken().size(); j++)
            {
                int realisation = rand() % onlookers[i].getCars()[car].getRealisationsTaken().size();   // id realizacji do zmodyfikowania
            }
        }
    }
}

// z losowych samochodow usuwamy pojedyncze realizacje, nastepnie probujemy je zapakowac do innych, tym, ktorych sie nie udalo
// losujemy nowe samochody
void BeeColony::switchRealisations(int coefficient)
{
    coefficient = rand() % coefficient + 3;
    for (int i = 0; i < onlookers.size(); i++)
    {
        vector<MyRealisation> freeRealisations;
        int realisationsToRemove = onlookers[i].getCars().size()/coefficient;

        while (realisationsToRemove != 0)
        {
            // losujemy samochod i realizacje do usuniecia
            int car = rand() % onlookers[i].getCars().size();
            int realisation = rand() % onlookers[i].getCars()[car].getRealisationsTaken().size();

            // zapamietujemy usunieta realizacje
            freeRealisations.push_back(network.getRealisations()[onlookers[i].getCars()[car].getRealisationsTaken()[realisation]]);

            // usuwamy realizacje z danego samochodu
            onlookers[i].removeRealisation(car, realisation, network);

            realisationsToRemove--;
        }
        //przydzielamy realizacje istniejacym samochodom
        freeRealisations = onlookers[i].fillTheCars(freeRealisations);

        // generujemy samochody dla pozostalych realizacji
        onlookers[i].generateCars(freeRealisations, this->allPaths);
    }
}

// operator na nowo przydzielajacy realizacje do istniejacych samochodow
void BeeColony::mixCars()
{
    for (int i = 0; i < onlookers.size(); i++)
    {
        // rozładowujemy samochody
        onlookers[i].unloadCars();
        // probujemy umiescic zwolnione realizacje w juz istniejacych samochodach
        onlookers[i].fillTheCarsBis(network.getRealisations(), this->allPaths);
    }
}

const vector<Bee> &BeeColony::getWorkers() const {
    return workers;
}