#include <iostream>
#include <random>
#include <fstream>
#include "Country.h"
#include "Network.h"
#include "Writer.h"
#include "BeeColony.h"
#include <ctime>
#include "DFS.h"
#include "EvolutionaryAlgorithm.h"
#include <chrono>

#define MAX_PATH_LENGTH 15

using namespace std;

int main() {
    Country country;
    double north = 55.0;
    double south = 50.25;
    double west = 10.0;
    double east = 40.0;
    const vector <int> beeColonyTresholdValues = {0, 3, 7, 17, 33, 67, 167, 333, 667, 1699};
    const vector <int> evolutionaryAlgorithmTresholdValues = {1, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 49999};
    vector<Path> all_paths;

    srand(time(NULL));

    cout << "MAX_PATH_LENGTH: " << MAX_PATH_LENGTH << endl;

    Network network(country, north, south, west, east);

    if (Network::check_crossroads(country.getCities()[0], country.getCities()[5], country.getCities()[5],
                                  country.getCities()[0]))
        cout << "Przecinaja sie" << endl;
    else
        cout << "Nie przecinaja sie" << endl;

    srand(time(nullptr));

    cout << network.getVertices().size() << endl;
    cout << network.getArcs().size() << endl;

    /* ************************************************************ Wypisywanie wygenerowanej sieci ***************** */
    for (int i = 0; i < network.getVertices().size(); i++) {
        cout << "vertex " << network.getVertices()[i].getName() << " arcsl: ";
        for (int j = 0; j < network.getVertices()[i].getArcsLeaving().size(); j++) {
            cout << network.getVertices()[i].getArcsLeaving()[j] << ", ";
        }
        cout << "; \n";

        cout << "vertex " << network.getVertices()[i].getName() << " arcse: ";
        for (int j = 0; j < network.getVertices()[i].getArcsEntering().size(); j++) {
            cout << network.getVertices()[i].getArcsEntering()[j] << ", ";
        }
        cout << "; \n";
    }

    for (int i = 0; i < network.getArcs().size(); i++) {
        cout << "arc " << i << " source: " << network.getArcs()[i].getSource().getName() << endl;
        cout << "arc " << i << " destination: " << network.getArcs()[i].getTarget().getName() << endl;
    }

//    for (int i = 0; i < network.getRealisations().size(); i++) {
//        cout << "Realisation " << i << " from " << network.getRealisations()[i].getSource().getName() << " to " <<
//             network.getRealisations()[i].getDestination().getName() << endl;
//    }

    Writer writer;
    writer.createExtendedDat(network);
    // writer.createJuneDat(network, 15);
    network.divideRealisations();

    cout << endl;

    /* ************************************************************* Przygotowywanie tras  ************************** */

    DFS dfs(network.getVertices(), network.getArcs(), network.getVertices().size());

    vector<Path> tmp_paths;
    for (int i = 0; i < network.getVertices().size() - 1; i++) {
        for (int j = i + 1; j < network.getVertices().size(); j++) {
            dfs.findpaths(network.getVertices()[i].getId(), network.getVertices()[j].getId());
            tmp_paths = dfs.pickSolutions(3, country);

            for (int k = 0; k < tmp_paths.size(); k++){
                if(tmp_paths[k].getVerticesVisited().size() <= MAX_PATH_LENGTH)
                    all_paths.push_back(tmp_paths[k]);
            }

            tmp_paths.clear();
            dfs.clearSolutions();
        }
        cout << "paths for city number " << i << " done" << endl;
    }

    /* ************************************************************ Bee Colony   ************************************ */
    vector<int> beeColoniesResults;
    int bestResult = INT_MAX;
    int avg = 0;

    cout << "Population 30, rp 40, iterations 1700, coefficients 3-12" << endl;
    auto begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 20; i++) {
        auto time_tmp_start = std::chrono::high_resolution_clock::now();
        BeeColony beeColony(30, network.getRealisations(), all_paths, 40, network);
        int beeColonyResult = beeColony.nectarHarvesting(1700, 10, 10, country, false, beeColonyTresholdValues);
        auto time_tmp_end = std::chrono::high_resolution_clock::now();
        beeColoniesResults.push_back(beeColonyResult);
        auto tmp_elapsed =  std::chrono::duration_cast<std::chrono::nanoseconds>(time_tmp_end - time_tmp_start);

        if (beeColonyResult < bestResult)
            bestResult = beeColonyResult;

        cout << "BeeColony " << i << " o wartosci " << beeColonyResult << " ukonczone. Ostatnio zmodyfikowane: " <<
             beeColony.getWorkers()[beeColony.getBestBeeIndex()].getCyclesWithoutUpdate();
        printf(". Czas BeeColony: %.3f sekund.\n", tmp_elapsed.count() * 1e-9);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    printf("Czas sumaryczny BeeColonies: %.3f sekund.\n", elapsed.count() * 1e-9);

    cout << "Najlepszy rezultat: " << endl;
    cout << bestResult << endl;

    for (int i = 0; i < beeColoniesResults.size(); i++)
        avg += beeColoniesResults[i];

    avg /= beeColoniesResults.size();
    cout << "Sredni rezultat BeeColonies: " << endl;
    cout << avg << endl;

    /* ************************************************************ Bee Colony   ************************************ */
//    beeColoniesResults.clear();
//    bestResult = INT_MAX;
//    avg = 0;
//
//    cout << "Population 30, rp 40, iterations 2000, coefficients 3-12" << endl;
//    begin = std::chrono::high_resolution_clock::now();
//    for (int i = 0; i < 5; i++)
//    {
//        BeeColony beeColony(30, network.getRealisations(), all_paths, 40, network);
//        int beeColonyResult = beeColony.nectarHarvesting(2000, 10, 10, country, false, beeColonyTresholdValues);
//        beeColoniesResults.push_back(beeColonyResult);
//
//        if (beeColonyResult < bestResult)
//            bestResult = beeColonyResult;
//
//        cout << "BeeColony " << i << " o wartosci " << beeColonyResult << " ukonczone. Ostatnio zmodyfikowane: "<<
//             beeColony.getWorkers()[beeColony.getBestBeeIndex()].getCyclesWithoutUpdate() << endl;
//    }
//    end = std::chrono::high_resolution_clock::now();
//    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//    printf("Czas sumaryczny BeeColonies: %.3f sekund.\n", elapsed.count() * 1e-9);
//
//    cout << "Najlepszy rezultat: " << endl;
//    cout << bestResult << endl;
//
//    for (int i = 0; i < beeColoniesResults.size(); i++)
//        avg += beeColoniesResults[i];
//
//    avg /= beeColoniesResults.size();
//    cout << "Sredni rezultat BeeColonies: " << endl;
//    cout << avg << endl;

    /* ************************************************************ Bee Colony   ************************************ */
//    beeColoniesResults.clear();
//    bestResult = INT_MAX;
//    avg = 0;
//
//    cout << "Population 30, rp 40, iterations 1700, coefficients 3-12" << endl;
//    begin = std::chrono::high_resolution_clock::now();
//    for (int i = 0; i < 5; i++)
//    {
//        BeeColony beeColony(30, network.getRealisations(), all_paths, 40, network);
//        int beeColonyResult = beeColony.nectarHarvesting(1700, 10, 10, country, false, beeColonyTresholdValues);
//        beeColoniesResults.push_back(beeColonyResult);
//
//        if (beeColonyResult < bestResult)
//            bestResult = beeColonyResult;
//
//        cout << "BeeColony " << i << " o wartosci " << beeColonyResult << " ukonczone. Ostatnio zmodyfikowane: "<<
//             beeColony.getWorkers()[beeColony.getBestBeeIndex()].getCyclesWithoutUpdate() << endl;
//    }
//    end = std::chrono::high_resolution_clock::now();
//    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//    printf("Czas sumaryczny BeeColonies: %.3f sekund.\n", elapsed.count() * 1e-9);
//
//    cout << "Najlepszy rezultat: " << endl;
//    cout << bestResult << endl;
//
//    for (int i = 0; i < beeColoniesResults.size(); i++)
//        avg += beeColoniesResults[i];
//
//    avg /= beeColoniesResults.size();
//    cout << "Sredni rezultat BeeColonies: " << endl;
//    cout << avg << endl;

    /* ************************************************************ Bee Colony   ************************************ */
//    beeColoniesResults.clear();
//    bestResult = INT_MAX;
//    avg = 0;
//
//    cout << "Population 35, rp 40, iterations 1500, coefficients 3-12" << endl;
//    begin = std::chrono::high_resolution_clock::now();
//    for (int i = 0; i < 5; i++)
//    {
//        BeeColony beeColony(35, network.getRealisations(), all_paths, 40, network);
//        int beeColonyResult = beeColony.nectarHarvesting(1500, 10, 10, country, false, beeColonyTresholdValues);
//        beeColoniesResults.push_back(beeColonyResult);
//
//        if (beeColonyResult < bestResult)
//            bestResult = beeColonyResult;
//
//        cout << "BeeColony " << i << " o wartosci " << beeColonyResult << " ukonczone. Ostatnio zmodyfikowane: "<<
//             beeColony.getWorkers()[beeColony.getBestBeeIndex()].getCyclesWithoutUpdate() << endl;
//    }
//    end = std::chrono::high_resolution_clock::now();
//    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//    printf("Czas sumaryczny BeeColonies: %.3f sekund.\n", elapsed.count() * 1e-9);
//
//    cout << "Najlepszy rezultat: " << endl;
//    cout << bestResult << endl;
//
//    for (int i = 0; i < beeColoniesResults.size(); i++)
//        avg += beeColoniesResults[i];
//
//    avg /= beeColoniesResults.size();
//    cout << "Sredni rezultat BeeColonies: " << endl;
//    cout << avg << endl;

    /* ************************************************************ Bee Colony   ************************************ */
//    beeColoniesResults.clear();
//    bestResult = INT_MAX;
//    avg = 0;
//
//    cout << "Population 40, rp 40, iterations 1250, coefficients 3-12" << endl;
//    begin = std::chrono::high_resolution_clock::now();
//    for (int i = 0; i < 5; i++)
//    {
//        BeeColony beeColony(40, network.getRealisations(), all_paths, 40, network);
//        int beeColonyResult = beeColony.nectarHarvesting(1250, 10, 10, country, false, beeColonyTresholdValues);
//        beeColoniesResults.push_back(beeColonyResult);
//
//        if (beeColonyResult < bestResult)
//            bestResult = beeColonyResult;
//
//        cout << "BeeColony " << i << " o wartosci " << beeColonyResult << " ukonczone. Ostatnio zmodyfikowane: "<<
//             beeColony.getWorkers()[beeColony.getBestBeeIndex()].getCyclesWithoutUpdate() << endl;
//    }
//    end = std::chrono::high_resolution_clock::now();
//    elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
//    printf("Czas sumaryczny BeeColonies: %.3f sekund.\n", elapsed.count() * 1e-9);
//
//    cout << "Najlepszy rezultat: " << endl;
//    cout << bestResult << endl;
//
//    for (int i = 0; i < beeColoniesResults.size(); i++)
//        avg += beeColoniesResults[i];
//
//    avg /= beeColoniesResults.size();
//    cout << "Sredni rezultat BeeColonies: " << endl;
//    cout << avg << endl;

    //* ****************************************    Evolutionary Algorithm   **************************************** */
    vector<int> EAResults;
    int bestResultEA = INT_MAX;
    int avgEA = 0;

    auto beginBis = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 20; i++) {
        auto time_tmp_start = std::chrono::high_resolution_clock::now();
        EvolutionaryAlgorithm evolutionaryAlgorithm(1, "DarwinBis", all_paths, network);
        evolutionaryAlgorithm.evolution(10000, 10, 10, country, false, evolutionaryAlgorithmTresholdValues);
        auto time_tmp_end = std::chrono::high_resolution_clock::now();
        cout << "Koszt: " << evolutionaryAlgorithm.getSpecimen().getCost() << " po " << evolutionaryAlgorithm.getGenerations()
             << " generacjach. Siec ma rozmiar: " << evolutionaryAlgorithm.getNetwork().getRealisations().size();
        auto tmp_elapsed =  std::chrono::duration_cast<std::chrono::nanoseconds>(time_tmp_end - time_tmp_start);
        printf(". Czas : %.3f sekund.\n", tmp_elapsed.count() * 1e-9);
        int resultEA = evolutionaryAlgorithm.getSpecimen().getRealCost();
        EAResults.push_back(resultEA);

        if (resultEA < bestResultEA)
            bestResultEA = resultEA;
    }
    auto endBis = std::chrono::high_resolution_clock::now();
    auto elapsedBis = std::chrono::duration_cast<std::chrono::nanoseconds>(endBis - beginBis);
    printf("Czas sumaryczny AE: %.3f sekund.\n", elapsedBis.count() * 1e-9);

    cout << "Najlepszy rezultat: " << endl;
    cout << bestResultEA << endl;

    for (int i = 0; i < EAResults.size(); i++)
        avgEA += EAResults[i];

    avgEA /= EAResults.size();
    cout << "Sredni rezultat EAs: " << endl;
    cout << avgEA << endl;

    //* ****************************************    Evolutionary Algorithm   **************************************** */
//    EAResults.clear();
//    bestResultEA = INT_MAX;
//    avgEA = 0;
//
//    beginBis = std::chrono::high_resolution_clock::now();
//    for (int i = 0; i < 5; i++)
//    {
//        EvolutionaryAlgorithm evolutionaryAlgorithm(1, "DarwinBis", all_paths, network);
//        evolutionaryAlgorithm.evolution(10000, 10, 10, country, false);
//        cout << "Koszt: " << evolutionaryAlgorithm.getSpecimen().getCost() << " po " << evolutionaryAlgorithm.getGenerations()
//             << " generacjach. Siec ma rozmiar: " << evolutionaryAlgorithm.getNetwork().getRealisations().size() << endl;
//        int resultEA = evolutionaryAlgorithm.getSpecimen().getRealCost();
//        EAResults.push_back(resultEA);
//
//        if (resultEA < bestResultEA)
//            bestResultEA = resultEA;
//    }
//    endBis = std::chrono::high_resolution_clock::now();
//    elapsedBis = std::chrono::duration_cast<std::chrono::nanoseconds>(endBis - beginBis);
//    printf("Czas sumaryczny AE: %.3f sekund.\n", elapsedBis.count() * 1e-9);
//
//    cout << "Najlepszy rezultat: " << endl;
//    cout << bestResultEA << endl;
//
//    for (int i = 0; i < EAResults.size(); i++)
//        avgEA += EAResults[i];
//
//    avgEA /= EAResults.size();
//    cout << "Sredni rezultat EAs: " << endl;
//    cout << avgEA << endl;

    return 0;
}
