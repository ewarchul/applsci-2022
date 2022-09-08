#include "Writer.h"

#define VEHICLES_NUMBER 3

void Writer::createExtendedDat(Network network) {
  ofstream myFile;
  myFile.open("extended-model.dat");

  // string vehicles[VEHICLES_NUMBER] = {"RENAULT", "FIAT", "TIR"};
  double vehicle_coefficients[VEHICLES_NUMBER] = {0.3, 0.5, 0.9};
  double time_coefficients[VEHICLES_NUMBER] = {70, 60, 50};

  // writing vertices ****************************************
  // ********************************
  myFile << "set VERT :=";
  for (int i = 0; i < network.getVertices().size(); i++) {
    char a = network.getVertices()[i].getName()[1];
    a = toupper(a);
    char b = network.getVertices()[i].getName()[2];
    b = toupper(b);
    myFile << "\t" << network.getVertices()[i].getCode();
  }
  myFile << ";\n\n";

  // writing arcs ******************************************
  // *********************************
  myFile << "set ARCS :=";
  for (int i = 0; i < network.getArcs().size(); i++) {
    myFile << "\t" << network.getArcs()[i].getCode();
  }
  myFile << ";\n\n";

  // writing realisations **********************************
  // *********************************
  myFile << "set STREAMS :=";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    myFile << "\t" << network.getRealisations()[i].getCode();
  }
  myFile << ";\n\n";

  // writing vehicles **************************************
  // ********************************
  myFile << "set VEHICLES :=";
  myFile << "\t" << network.getVehicles()[0] << "\t" << network.getVehicles()[1]
         << "\t" << network.getVehicles()[2] << ";\n\n";

  // writing dimensions   **********************************
  // ********************************
  myFile << "set DIM :=";
  myFile << "\t" << network.getDim()[0] << "\t" << network.getDim()[1]
         << ";\n\n";

  // writing logistic function    **************************
  // ********************************
  myFile << "set LOG_FUN :=";
  myFile << "\t" << network.getLogFun()[0] << "\t" << network.getLogFun()[1]
         << "\t" << network.getLogFun()[2] << ";\n\n";

  // writing delays   **************************************
  // ********************************
  myFile << "set DELAYS :=";
  myFile << "\t" << network.getDelays()[0] << "\t" << network.getDelays()[1]
         << "\t" << network.getDelays()[2] << "\t" << network.getDelays()[3]
         << ";\n\n";

  myFile << ";\n\n";

  // writing arcs entering    ******************************
  // ********************************
  for (int i = 0; i < network.getVertices().size(); i++) {
    myFile << "set ARCSE[";
    myFile << network.getVertices()[i].getCode() << "] :=";
    for (int j = 0; j < network.getVertices()[i].getArcsEntering().size();
         j++) {
      for (int k = 0; k < network.getArcs().size(); k++) {
        if (network.getArcs()[k].getId() ==
            network.getVertices()[i].getArcsEntering()[j]) {
          myFile << " " << network.getArcs()[k].getCode();
        }
      }
    }
    myFile << ";\n";
  }
  myFile << "\n";

  // writing arcs leaving    *******************************
  // ********************************
  for (int i = 0; i < network.getVertices().size(); i++) {
    myFile << "set ARCSL[";
    myFile << network.getVertices()[i].getCode() << "] :=";
    for (int j = 0; j < network.getVertices()[i].getArcsLeaving().size(); j++) {
      for (int k = 0; k < network.getArcs().size(); k++) {
        if (network.getArcs()[k].getId() ==
            network.getVertices()[i].getArcsLeaving()[j]) {
          myFile << " " << network.getArcs()[k].getCode();
        }
      }
    }
    myFile << ";\n";
  }
  myFile << "\n";

    // writing stream source    ******************************
    // ********************************
    myFile << "param source :=\n";
    for (int i = 0; i < network.getRealisations().size(); i++) {
        myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
               << network.getRealisations()[i].getSource().getCode() << "\n";
    }
    myFile << ";\n\n";

    // writing stream destination   **************************
    // ********************************
    myFile << "param dest :=\n";
    for (int i = 0; i < network.getRealisations().size(); i++) {
        myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
               << network.getRealisations()[i].getDestination().getCode() << "\n";
    }
    myFile << ";\n\n";

  // writing stream size ***********************************
  // ********************************
  myFile << "param stream_size :\tM\tV\t:=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    double size = network.getRealisations()[i].getVolume();
    myFile << "\t\t" << network.getRealisations()[i].getCode() << "\t" << size
           << "\t" << size << "\n";
  }
  myFile << ";\n\n";

  // writing max delay    **************************************
  // ********************************
  myFile << "param max_delay :=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
      myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
      << "D2"
      << "\n";
  }
  myFile << ";\n\n";

  // writing travel cost  **********************************
  // ********************************
  myFile << "param travel_cost (tr):";
  for (int i = 0; i < VEHICLES_NUMBER; i++) {
    myFile << "\t" << network.getVehicles()[i];
  }
  myFile << " :=\n";

  for (int i = 0; i < network.getArcs().size(); i++) {
    myFile << "\t\t" << network.getArcs()[i].getCode();
    myFile
        << "\t"
        << double(
               network.getArcs()[i].getLength() /* vehicle_coefficients[0]*/ /
               9.99);
    myFile
        << "\t"
        << double(
               network.getArcs()[i].getLength() /* vehicle_coefficients[1]*/ /
               6.67);
    myFile
        << "\t"
        << double(
               network.getArcs()[i].getLength() /* vehicle_coefficients[2]*/ /
               3.33)
        << "\n";
  }
  myFile << ";\n\n";

  // writing travel time  *********************************
  // ********************************
  myFile << "param travel_time (tr):";
  for (int i = 0; i < VEHICLES_NUMBER; i++) {
    myFile << "\t" << network.getVehicles()[i];
  }
  myFile << " :=\n";
  for (int i = 0; i < network.getArcs().size(); i++) {
    myFile << "\t\t" << network.getArcs()[i].getCode();
    myFile << "\t"
           << double(network.getArcs()[i].getLength() / time_coefficients[0]);
    myFile << "\t"
           << double(network.getArcs()[i].getLength() / time_coefficients[1]);
    myFile << "\t"
           << double(network.getArcs()[i].getLength() / time_coefficients[2])
           << "\n";
  }
  myFile << ";\n\n";

  // writing function cost    ******************************
  // ********************************
  myFile << "param function_cost:\tWER\tPER:=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    for (int j = 0; j < network.getVertices().size(); j++) {
      myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
             << network.getVertices()[j].getCode() << "\t"
             << "10"
             << "\t"
             << "10\n";
    }
  }
  myFile << ";\n\n";

  // writing function time    *****************************
  // ********************************
  myFile << "param function_time:\tWER\tPER:=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    for (int j = 0; j < network.getVertices().size(); j++) {
      myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
             << network.getVertices()[j].getCode() << "\t"
             << "1"
             << "\t"
             << "1\n";
    }
  }
  myFile << ";\n\n";

  // writing capacity **************************************
  // ********************************
  myFile << "param capacity (tr): ";
  for (int i = 0; i < VEHICLES_NUMBER; i++) {
      myFile << "\t" << network.getVehicles()[i];
  }
  myFile << " :=\n";
  myFile << "\t\tM\t25\t50\t100\n";
  myFile << "\t\tV\t25\t50\t100\n";
  myFile << ";\n\n";

  // writing next delay   **********************************                          ********************************
  myFile << "param next_delay :=\n";
  myFile << "\tDEL" << "\tD0\n";
  myFile << "\tD0" << "\tD1\n";
  myFile << "\tD1" << "\tD2\n";
  myFile << "\tD2" << "\tD3\n";
  myFile << ";\n\n";

  // writing next function    ******************************                          ********************************
  myFile << "param next_function :=\n";
  myFile << "\tNONE" << "\tWER\n";
  myFile << "\tWER" << "\tPER\n";
  myFile << "\tPER" << "\tNONE\n";
  myFile << ";\n\n";

  // writing gathered     **********************************
  // ********************************
  myFile << "param gathered :=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
           << network.getRealisations()[i].getCode() << "\n";
  }
  myFile << ";\n\n";

  // writing available time    **********************************
  // ********************************
  myFile << "param available_time :\t";
  for (int i = 0; i < network.getDelays().size(); i++)
      myFile << "\t" << network.getDelays()[i];
  myFile << " :=\n";

  for (int i = 0; i < network.getRealisations().size(); i++)
  {
      myFile << "\t\t" << network.getRealisations()[i].getCode();
      for (int j = 0; j < network.getDelays().size(); j++)
          myFile << "\t" << 24 - (network.getDelays().size() - j - 1) * 2;
      myFile << "\n";
  }
  myFile << ";\n\n";
}

void Writer::createFirstModelDat(Network network, int demand_max_value) {
  ofstream myFile;
  myFile.open(
      "base-model.dat");

  myFile << "set VERT :=";
  for (int i = 0; i < network.getVertices().size(); i++) {
    char a = network.getVertices()[i].getName()[1];
    a = toupper(a);
    char b = network.getVertices()[i].getName()[2];
    b = toupper(b);
    myFile << "\t" << network.getVertices()[i].getCode();
  }
  myFile << ";\n\n";

  // writing arcs ******************************************
  // *********************************
  myFile << "set ARCS :=";
  for (int i = 0; i < network.getArcs().size(); i++) {
    myFile << "\t" << network.getArcs()[i].getCode();
  }
  myFile << ";\n\n";

  // writing realisations **********************************
  // *********************************
  myFile << "set STREAMS :=";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    myFile << "\t" << network.getRealisations()[i].getCode();
  }
  myFile << ";\n\n";

  // writing vehicles **************************************
  // ********************************
  myFile << "set VEHICLES :=";
  myFile << "\t" << network.getVehicles()[0] << "\t" << network.getVehicles()[1]
         << "\t" << network.getVehicles()[2] << ";\n\n";

  // writing dimensions   **********************************
  // ********************************
  myFile << "set DIM :=";
  myFile << "\t" << network.getDim()[0] << "\t" << network.getDim()[1]
         << ";\n\n";

  // writing logistic function    **************************
  // ********************************
  myFile << "set LOG_FUN :=";
  myFile << "\t" << network.getLogFun()[0] << "\t" << network.getLogFun()[1]
         << "\t" << network.getLogFun()[2] << ";\n\n";

  // writing arcs entering    ******************************
  // ********************************
  for (int i = 0; i < network.getVertices().size(); i++) {
    myFile << "set ARCSE[";
    myFile << network.getVertices()[i].getCode() << "] :=";
    for (int j = 0; j < network.getVertices()[i].getArcsEntering().size();
         j++) {
      for (int k = 0; k < network.getArcs().size(); k++) {
        if (network.getArcs()[k].getId() ==
            network.getVertices()[i].getArcsEntering()[j]) {
          myFile << " " << network.getArcs()[k].getCode();
        }
      }
    }
    myFile << ";\n";
  }
  myFile << "\n";

  // writing arcs leaving    *******************************
  // ********************************
  for (int i = 0; i < network.getVertices().size(); i++) {
    myFile << "set ARCSL[";
    myFile << network.getVertices()[i].getCode() << "] :=";
    for (int j = 0; j < network.getVertices()[i].getArcsLeaving().size(); j++) {
      for (int k = 0; k < network.getArcs().size(); k++) {
        if (network.getArcs()[k].getId() ==
            network.getVertices()[i].getArcsLeaving()[j]) {
          myFile << " " << network.getArcs()[k].getCode();
        }
      }
    }
    myFile << ";\n";
  }
  myFile << "\n";

  // writing stream source    ******************************
  // ********************************
  myFile << "param source :=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
           << network.getRealisations()[i].getSource().getCode() << "\n";
  }
  myFile << ";\n\n";

  // writing stream destination   **************************
  // ********************************
  myFile << "param dest :=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
           << network.getRealisations()[i].getDestination().getCode() << "\n";
  }
  myFile << ";\n\n";

  // writing stream size ***********************************
  // ********************************
  myFile << "param stream_size :\tM\tV\t:=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    double size = network.getRealisations()[i].getVolume();
    myFile << "\t\t" << network.getRealisations()[i].getCode() << "\t" << size
           << "\t" << size << "\n";
  }
  myFile << ";\n\n";

  // writing max delay    **************************************
  // ********************************
  myFile << "param max_delay :=\n";
  for (int i = 0; i < network.getRealisations().size(); i++) {
    myFile << "\t" << network.getRealisations()[i].getCode() << "\t"
           << "D3"
           << "\n";
  }
  myFile << ";\n\n";

  // writing travel cost  **********************************
  // ********************************
  myFile << "param travel_cost (tr):";
  for (int i = 0; i < VEHICLES_NUMBER; i++) {
    myFile << "\t" << network.getVehicles()[i];
  }
  myFile << " :=\n";

  for (int i = 0; i < network.getArcs().size(); i++) {
    myFile << "\t\t" << network.getArcs()[i].getCode();
    myFile << "\t"
           << (network.getArcs()[i].getLength() /* vehicle_coefficients[0]*/ /
               9.99);
    myFile << "\t"
           << (network.getArcs()[i].getLength() /* vehicle_coefficients[1]*/ /
               6.67);
    myFile << "\t"
           << (network.getArcs()[i].getLength() /* vehicle_coefficients[2]*/ /
               3.33)
           << "\n";
  }
  myFile << ";\n\n";

  // writing capacity **************************************
  // ********************************
  myFile << "param capacity :=\n";
  myFile << "\t" << network.getVehicles()[0] << "\t25\n";
  myFile << "\t" << network.getVehicles()[1] << "\t50\n";
  myFile << "\t" << network.getVehicles()[2] << "\t100\n";
  myFile << ";\n\n";

  // writing next delay   **********************************
  // ********************************
  myFile << "next delay :=\n";
  myFile << "\tDEL"
         << "\tD0\n";
  myFile << "\tD0"
         << "\tD1\n";
  myFile << "\tD1"
         << "\tD2\n";
  myFile << "\tD2"
         << "\tD3\n";
  myFile << ";\n\n";

  // writing next function    ******************************
  // ********************************
  myFile << "next function :=\n";
  myFile << "\tNONE"
         << "\tWER\n";
  myFile << "\tWER"
         << "\tPER\n";
  myFile << "\tPER"
         << "\tNONE\n";
  myFile << ";\n\n";

  myFile.close();
}
