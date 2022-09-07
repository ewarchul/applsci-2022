#include "Network.h"

#define MAX_ARC_LENGTH 100
#define NUMBER_OF_DIVISIONS_PER_REALISATION 1000

using namespace std;

Network::Network(const Country &country, double north, double south, double west, double east)
{
    this->country = country;
    for (int i = 0; i < this->country.getCities().size(); i++)
    {
        if (this->country.getCities()[i].getXCoor() < north && this->country.getCities()[i].getXCoor() > south &&
                this->country.getCities()[i].getYCoor() > west && this->country.getCities()[i].getYCoor() < east)
        {
            vertices.push_back(this->country.getCities()[i]);
        }
    }

    this->generate_arcs(2.5, 5);
    this->generate_realisations(10, this->country);

    // *****************************************    arcs_entering   ****************************************************
    for (int i = 0; i < this->getVertices().size(); i++)
    {
        vector<MyArc> tmp;
        for (int j = 0; j < this->getVertices()[i].getArcsEntering().size(); j++)
        {
            for (int k = 0; k < this->getArcs().size(); k++)
            {
                if (this->getArcs()[k].getId() == this->getVertices()[i].getArcsEntering()[j])
                    tmp.emplace_back(this->getArcs()[k]);
            }
        }
        arcs_entering.emplace_back(tmp);
        tmp.clear();
    }

    // *****************************************    arcs_leaving   *****************************************************
    for (int i = 0; i < this->getVertices().size(); i++)
    {
        vector<MyArc> tmp;
        for (int j = 0; j < this->getVertices()[i].getArcsLeaving().size(); j++)
        {
            for (int k = 0; k < this->getArcs().size(); k++)
            {
                if (this->getArcs()[k].getId() == this->getVertices()[i].getArcsLeaving()[j])
                    tmp.emplace_back(this->getArcs()[k]);
            }
        }
        arcs_leaving.emplace_back(tmp);
        tmp.clear();
    }

    // *****************************************    arcs_for_vehicle   *************************************************
    for (int i = 0; i < VEHICLES_NUMBER; i++)
    {
        vector<MyArc> tmp;
        for (int j = 0; j < this->getArcs().size(); j++)
            tmp.emplace_back(this->getArcs()[j]);

        arcs_for_vehicle.emplace_back(tmp);
        tmp.clear();
    }

    // *****************************************    vehicles_for_vertex   **********************************************
    for (int i = 0; i < this->getVertices().size(); i++)
    {
        vector<string> tmp;
        for (int j = 0; j < VEHICLES_NUMBER; j++)
            tmp.emplace_back(vehicles[j]);

        vehicles_for_vertex.emplace_back(tmp);
        tmp.clear();
    }

    // *****************************************    streams_sizes   ****************************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
    {
        vector<int> tmp;
        tmp.push_back(this->getRealisations()[i].getVolume());
        tmp.push_back(this->getRealisations()[i].getVolume());
        streams_sizes.push_back(tmp);
        tmp.clear();
    }

    // *****************************************    realisation_source   ***********************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
        realisation_source.emplace_back(this->getRealisations()[i].getSource());

    // *****************************************    realisation_destination   ******************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
        realisation_target.emplace_back(this->getRealisations()[i].getDestination());

    // *****************************************    travel_cost   ******************************************************
    for (int i = 0; i < this->getArcs().size(); i++)
    {
        vector<int> tmp;
        for (int j = 0; j < VEHICLES_NUMBER; j++)
        {
            tmp.emplace_back(this->getArcs()[i].getLength() /* vehicle_coefficients[j]*/ / 10);    // porownac z writerem
        }
        travel_cost.emplace_back(tmp);
        arcs[i].setTravelCost(tmp);
        tmp.clear();
    }

    // *****************************************    travel_time   ******************************************************
    for (int i = 0; i < this->getArcs().size(); i++)
    {
        vector<int> tmp;
        for (int j = 0; j < VEHICLES_NUMBER; j++)
        {
            tmp.emplace_back( 8 /*this->getArcs()[i].getLength() * time_coefficients[j] / 10*/);       // porownac z writerem
        }
        travel_time.emplace_back(tmp);
        tmp.clear();
    }

    // *****************************************    function_cost   ****************************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
    {
        vector<int> tmp;
        vector<vector<int>> tmpBis;
        for (int j = 0; j < this->getVertices().size(); j++)
        {
            tmp.emplace_back(10);    // WER cost
            tmp.emplace_back(10);    // PER cost
            tmpBis.emplace_back(tmp);
        }
        function_cost.emplace_back(tmpBis);
        tmp.clear();
        tmpBis.clear();
    }

    // *****************************************    function_time   ****************************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
    {
        vector<int> tmp;
        vector<vector<int>> tmpBis;
        for (int j = 0; j < this->getVertices().size(); j++)
        {
            tmp.emplace_back(1);    // WER time
            tmp.emplace_back(1);    // PER time
            tmpBis.emplace_back(tmp);
        }
        function_time.emplace_back(tmpBis);
        tmp.clear();
        tmpBis.clear();
    }

    // *****************************************    realisation_gathered   *********************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
    {
        vector<MyRealisation> tmp;
        tmp.emplace_back(this->getRealisations()[i]);

        realisation_gathered.emplace_back(tmp);
        tmp.clear();
    }

    // *****************************************    vertex_open   ******************************************************
    int opened_times[this->getVertices().size()];
    for (int i = 0; i < this->getVertices().size(); i++)
    {
        opened_times[i] = rand()%6 + 6;
        vertex_open.emplace_back(opened_times[i]);
    }

    // *****************************************    vertex_closed   ****************************************************
    for (int i = 0; i < this->getVertices().size(); i++)
        vertex_closed.emplace_back(24 - opened_times[i]);

    // *****************************************    realisation_available   ********************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
        realisation_available.emplace_back(4);

    // *****************************************    realisation_delivery   *********************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
        realisation_delivery.emplace_back(8);

    // *****************************************    realisation_max_delay   ********************************************
    for (int i = 0; i < this->getRealisations().size(); i++)
        realisation_max_delay.emplace_back("D2");

    // *****************************************    period_time   ******************************************************
    for (int i = 0; i < PERIODS_NUMBER; i++)
    {
        period_time.emplace_back(24);
    }
}

void Network::generate_arcs(double coefficient, int max_vertices)
{
    vector<MyVertex> sortedCities;
    vector<int> sortedCitiesChecklist;
    vector<int> arc_sources;
    vector<int> arc_targets;
    int arc_counter = 0;

    for (int i = 0; i < vertices.size(); i++)
        sortedCitiesChecklist.emplace_back(0);

    for (int i = 0; i < vertices.size(); i++)
    {
        int northest = -1;
        double northest_coor = 0.0;
        for (int j = 0; j < vertices.size(); j++)
        {
            if (vertices[j].getXCoor() > northest_coor && sortedCitiesChecklist[j] == 0)
            {
                northest = j;
                northest_coor = vertices[j].getXCoor();
            }
        }
        sortedCitiesChecklist[northest] = 1;
        sortedCities.push_back(vertices[northest]);
    }

    sortedCities[0].addArcsl(0);
    sortedCities[0].addArcse(1);
    arc_sources.emplace_back(0);
    arc_targets.emplace_back(1);

    sortedCities[1].addArcsl(1);
    sortedCities[1].addArcse(0);
    arc_sources.emplace_back(1);
    arc_targets.emplace_back(0);
    arc_counter += 2;

    for (int i = 2; i < sortedCities.size(); i++)
    {
        int closest = -1;
        int min_distance = 10000;

        for (int j = 0; j < i; j++)
        {
            bool result = false;
            if ((MyArc::calculateLength(sortedCities[i], sortedCities[j]) < min_distance))
            {
                for (int k = 0; k < arc_sources.size(); k++)
                {
                    if (check_crossroads(sortedCities[i], sortedCities[j], sortedCities[arc_sources[k]], sortedCities[arc_targets[k]]))     // pamietaj o max_vertices
                    {
                        result = true;
                        break;
                    }
                }
                if (result)
                    continue;

                closest = j;
                min_distance = MyArc::calculateLength(sortedCities[i], sortedCities[j]);
            }
        }
        if (closest == -1)
        {
            cout << "Unable to create network." << endl;
            exit(0);
        }

        sortedCities[closest].addArcsl(arc_counter);
        sortedCities[closest].addArcse(arc_counter+1);
        arc_sources.emplace_back(closest);
        arc_targets.emplace_back(i);

        sortedCities[i].addArcsl(arc_counter+1);
        sortedCities[i].addArcse(arc_counter);
        arc_sources.emplace_back(i);
        arc_targets.emplace_back(closest);
        arc_counter += 2;
    }

    int counterBis = sortedCities.size() * 2 - 2 ;
    for (int i = counterBis; i < sortedCities.size()*coefficient; i += 2)
    {
        srand(time(nullptr));
        int src = rand() % sortedCities.size();
        int dest = rand() % sortedCities.size();
        bool result = false;
        for (int j = 0; j < arc_targets.size(); j++)
        {
            result = check_crossroads(sortedCities[src], sortedCities[dest], sortedCities[arc_sources[j]], sortedCities[arc_targets[j]]);
            if (result)
                break;
        }

        if (result)
        {
            i -= 2;
            continue;
        }

        if (sortedCities[src].getArcsLeaving().size() >= max_vertices || sortedCities[dest].getArcsEntering().size() >= max_vertices )
        {
            i -= 2;
            continue;
        }

        if (src == dest)
        {
            i -= 2;
            continue;
        }

        if (abs(sortedCities[src].getXCoor() - sortedCities[dest].getXCoor()) > abs(sortedCities[src].getYCoor() - sortedCities[dest].getYCoor()))
        {
            i -= 2;
            continue;
        }

        sortedCities[src].addArcsl(arc_counter);
        sortedCities[src].addArcse(arc_counter+1);
        arc_sources.emplace_back(src);
        arc_targets.emplace_back(dest);

        sortedCities[dest].addArcsl(arc_counter+1);
        sortedCities[dest].addArcse(arc_counter);
        arc_sources.emplace_back(dest);
        arc_targets.emplace_back(src);
        arc_counter += 2;
    }

    for (int i = 0; i < sortedCities.size(); i++)                   // kopiowanie zawartosci arcse i arcsl z sorted cities do vertices
    {
        for (int j = 0; j < vertices.size(); j++)
        {
            if (sortedCities[i].getId() == vertices[j].getId())
            {
                for (int k = 0; k < sortedCities[i].getArcsLeaving().size(); k++)
                {
                    vertices[j].addArcsl(sortedCities[i].getArcsLeaving()[k]);
                    vertices[j].addArcse(sortedCities[i].getArcsEntering()[k]);
                }
            }
        }
    }

    for(int i = 0; i < arc_counter; i += 2)                     // tworzenie arcsow na podstawie informacji zawartych w
    {                                                           // tablicach arcsl i arcse rekordow tablicy vertices
        int dest, src;
        for(int j = 0; j < vertices.size(); j++)
        {
            for(int k = 0; k < vertices[j].getArcsLeaving().size(); k++)
            {
                if(vertices[j].getArcsLeaving()[k] == i)
                    src = j;
            }
            for(int k = 0; k < vertices[j].getArcsEntering().size(); k++)
            {
                if(vertices[j].getArcsEntering()[k] == i)
                    dest = j;
            }
        }
        arcs.emplace_back(i, vertices[src], vertices[dest]);
        arcs.emplace_back(i + 1, vertices[dest], vertices[src]);
    }
}

void Network::generate_realisations(int max_value, const Country &country)
{
    int counter = 0;
    for (int i = 0; i < this->vertices.size(); i++)
    {
        for (int j = i + 1; j < this->vertices.size(); j++)
        {
            DFS dfs(this->vertices, this->arcs, this->vertices.size());
            dfs.findpaths(this->vertices[i].getId(), this->vertices[j].getId());
            vector<Path> shortestPath = dfs.pickSolutions(1, country);
            int value = rand() % max_value + 5;
            this->realisations.emplace_back(counter, vertices[i], vertices[j], value, shortestPath[0]);
            counter++;
        }
    }

//    int counter = 0;
//    for (int i = 0; i < vertices.size()*(this->vertices.size()-1)/2; i++)
//    {
//        int src = rand() % vertices.size();
//        int dest = rand() % vertices.size();
//        int value = rand() % max_value + 5;
//        bool allowed = true;
//
//        for (int j = 0; j < realisations.size(); j++)
//        {
//            if ((vertices[src].getId() == realisations[j].getSource().getId() && vertices[dest].getId() == realisations[j].getDestination().getId()) ||
//                src == dest)
//            {
//                allowed = false;
//                break;
//            }
//        }
//
//        if (allowed == false)
//        {
//            i = i - 1;
//            continue;
//        }
//
//        realisations.emplace_back(counter, vertices[src], vertices[dest], value);
//        counter++;
//    }
}

void Network::divideRealisations()
{
    int initialRealisationsNumber = this->realisations.size();
    for (int i = 0; i < NUMBER_OF_DIVISIONS_PER_REALISATION * initialRealisationsNumber; i++)
    {
        int realisationToDivide = rand() % this->realisations.size();
        Path chosenPath = this->realisations[realisationToDivide].getShortestPath();

        if (chosenPath.getVerticesVisited().size() >= 4)
        {
            // losujemy punkt, w którym podzielimy realizację i tworzymy na tej podstawie 2 nowe
            int cutPoint = rand() % (chosenPath.getVerticesVisited().size() - 2) + 1;
            MyVertex chosenVertex = this->vertices[chosenPath.getVerticesVisited()[cutPoint]];
            int sumOfArcs = chosenVertex.getArcsLeaving().size() + chosenVertex.getArcsEntering().size();

            if (sumOfArcs > 8) {
                MyRealisation firstHalf(realisationToDivide, realisations[realisationToDivide].getSource(), country.getCities()[this->realisations[realisationToDivide].getShortestPath().getVerticesVisited()[cutPoint]],
                                        realisations[realisationToDivide].getMass(), vector<int> (chosenPath.getVerticesVisited().begin(),
                                                                                                  chosenPath.getVerticesVisited().begin() + cutPoint + 1));
                MyRealisation secondHalf(realisations.size(), country.getCities()[this->realisations[realisationToDivide].getShortestPath().getVerticesVisited()[cutPoint]], realisations[realisationToDivide].getDestination(),
                                         realisations[realisationToDivide].getMass(), vector<int> (chosenPath.getVerticesVisited().begin() + cutPoint,
                                                                                                   chosenPath.getVerticesVisited().end()));

                // usuwamy realizacje ktora zostala podzielona
                realisations.erase(realisations.begin() + realisationToDivide);

                // do wektora realizacji dodajemy 2 nowo utworzone
                realisations.insert(realisations.begin() + realisationToDivide, firstHalf); // pierwsza wchodzi na miejsce poprzedniej aby uniknac zmiany indeksow
                realisations.push_back(secondHalf);
            }
        }
    }
}

bool Network::check_crossroads(const MyVertex& src, const MyVertex& dst, const MyVertex& srcBis, const MyVertex& dstBis)
{
    if((src.getXCoor() == srcBis.getXCoor() && src.getYCoor() == srcBis.getYCoor()) && !(dst.getXCoor() == dstBis.getXCoor() && dst.getYCoor() == dstBis.getYCoor()))
        return false;
    if(!(src.getXCoor() == srcBis.getXCoor() && src.getYCoor() == srcBis.getYCoor()) && (dst.getXCoor() == dstBis.getXCoor() && dst.getYCoor() == dstBis.getYCoor()))
        return false;
    if((src.getXCoor() == dstBis.getXCoor() && src.getYCoor() == dstBis.getYCoor()) && !(dst.getXCoor() == srcBis.getXCoor() && dst.getYCoor() == srcBis.getYCoor()))
        return false;
    if(!(src.getXCoor() == dstBis.getXCoor() && src.getYCoor() == dstBis.getYCoor()) && (dst.getXCoor() == srcBis.getXCoor() && dst.getYCoor() == srcBis.getYCoor()))
        return false;


    double s1 = multiplication(src, srcBis, dst);
    double s2 = multiplication(src, dstBis, dst);
    double s3 = multiplication(srcBis, src, dstBis);
    double s4 = multiplication(srcBis, dst, dstBis);

    if (((s1 > 0 && s2 < 0) || (s1 < 0 && s2 > 0)) && ((s3 < 0 && s4 > 0) || (s3 > 0 && s4 < 0)))
        return true;
    else if (s1 == 0 && lies_between(src, dst, srcBis))
        return true;
    else if (s2 == 0 && lies_between(src, dst, dstBis))
        return true;
    else if (s3 == 0 && lies_between(srcBis, dstBis, src))
        return true;
    else if (s4 == 0 && lies_between(srcBis, dstBis, dst))
        return true;
    else
        return false;
}

double Network::multiplication(const MyVertex& v1, const MyVertex& v2, const MyVertex& v3)
{
    return (v2.getXCoor() - v1.getXCoor()) * (v3.getYCoor() - v1.getYCoor()) - (v3.getXCoor() - v1.getXCoor()) * (v2.getYCoor() - v1.getYCoor());
}

bool Network::lies_between(const MyVertex& v1, const MyVertex& v2, const MyVertex& v3)
{
    if (fmin(v1.getXCoor(), v2.getXCoor()) <= v3.getXCoor() && v3.getXCoor() <= fmax(v1.getXCoor(), v2.getXCoor()))
        return true;   //zakładamy, że p1,p2 i p3 są współliniowe
    else
        return false;
}

// obliczamy najkrotsza trase dla losowej realizacji, a następnie dzielimy ją na 2 mniejsze
vector<MyRealisation> Network::divideRandomRealisation(int realIndex)
{
    //int realIndex = rand() % this->realisations.size();
    Path chosenPath = this->realisations[realIndex].getShortestPath();
    vector<MyRealisation> createdRealisations;

    // losujemy tylko raz, dzieki temu jesli wiekszosc realizacji bedzie krotka to ciezko bedzie je dzielic dalej
    if (chosenPath.getVerticesVisited().size() >= 3)
    {
        // losujemy punkt, w którym podzielimy realizację i tworzymy na tej podstawie 2 nowe
        int cutPoint = rand() % (chosenPath.getVerticesVisited().size() - 2) + 1;
//        MyVertex chosenVertex = this->vertices[chosenPath.getVerticesVisited()[cutPoint]];
//        int sumOfArcs = chosenVertex.getArcsLeaving().size() + chosenVertex.getArcsEntering().size();

        MyRealisation firstHalf(realIndex, realisations[realIndex].getSource(), country.getCities()[this->realisations[realIndex].getShortestPath().getVerticesVisited()[cutPoint]],
                                realisations[realIndex].getMass(), vector<int> (chosenPath.getVerticesVisited().begin(),
                                                                                chosenPath.getVerticesVisited().begin() + cutPoint + 1));
        MyRealisation secondHalf(realisations.size(), country.getCities()[this->realisations[realIndex].getShortestPath().getVerticesVisited()[cutPoint]], realisations[realIndex].getDestination(),
                                 realisations[realIndex].getMass(), vector<int> (chosenPath.getVerticesVisited().begin() + cutPoint,
                                                                                 chosenPath.getVerticesVisited().end()));

        // zmniejszamy id wszystkich realizacji, które znajdują się za dzieloną, ponieważ będzie ona usunięta
//        for (int i = realIndex; i < realisations.size(); i++)
//            realisations[i].setId(realisations[i].getId() - 1);

        // usuwamy realizacje ktora zostala podzielona
        realisations.erase(realisations.begin() + realIndex);

        // do wektora realizacji dodajemy 2 nowo utworzone
        realisations.insert(realisations.begin() + realIndex, firstHalf); // pierwsza wchodzi na miejsce poprzedniej aby uniknac zmiany indeksow
        createdRealisations.push_back(firstHalf);
        realisations.push_back(secondHalf);
        createdRealisations.push_back(secondHalf);
    }
    return createdRealisations;
}

const vector<MyVertex> &Network::getVertices() const {
    return vertices;
}

const vector<MyArc> &Network::getArcs() const {
    return arcs;
}

const vector<MyRealisation> &Network::getRealisations() const {
    return realisations;
}

const string *Network::getPeriods() const {
    return periods;
}

const double *Network::getVehicleCoefficients() const {
    return vehicle_coefficients;
}

const double *Network::getTimeCoefficients() const {
    return time_coefficients;
}

const string *Network::getVehicles() const {
    return vehicles;
}

const string *Network::getDim() const {
    return dim;
}

const string *Network::getLogFun() const {
    return log_fun;
}

const string *Network::getDelays() const {
    return delays;
}

const vector<vector<MyArc>> &Network::getArcsEntering() const {
    return arcs_entering;
}

const vector<vector<MyArc>> &Network::getArcsLeaving() const {
    return arcs_leaving;
}

const vector<vector<MyArc>> &Network::getArcsForVehicle() const {
    return arcs_for_vehicle;
}

const vector<vector<string>> &Network::getVehiclesForVertex() const {
    return vehicles_for_vertex;
}

const vector<vector<int>> &Network::getStreamsSizes() const {
    return streams_sizes;
}

const vector<MyVertex> &Network::getRealisationSource() const {
    return realisation_source;
}

const vector<MyVertex> &Network::getRealisationTarget() const {
    return realisation_target;
}

const vector<vector<int>> &Network::getTravelCost() const {
    return travel_cost;
}

const vector<vector<int>> &Network::getTravelTime() const {
    return travel_time;
}

const int *Network::getCapacity() const {
    return capacity;
}

const vector<vector<vector<int>>> &Network::getFunctionCost() const {
    return function_cost;
}

const vector<vector<vector<int>>> &Network::getFunctionTime() const {
    return function_time;
}

const vector<vector<MyRealisation>> &Network::getRealisationGathered() const {
    return realisation_gathered;
}

const vector<int> &Network::getVertexOpen() const {
    return vertex_open;
}

const vector<int> &Network::getVertexClosed() const {
    return vertex_closed;
}

const vector<int> &Network::getRealisationAvailable() const {
    return realisation_available;
}

const vector<int> &Network::getRealisationDelivery() const {
    return realisation_delivery;
}

const vector<string> &Network::getRealisationMaxDelay() const {
    return realisation_max_delay;
}

const vector<int> &Network::getPeriodTime() const {
    return period_time;
}

const int Network::getCity(int index)
{
    for(int i = 0; i < this->vertices.size(); i++)
    {
        if(index == this->vertices[i].getId())
            return i;
    }
}

Network::Network() {}
