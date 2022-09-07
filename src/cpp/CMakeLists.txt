cmake_minimum_required(VERSION 3.16)
project(NetworkGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(NetworkGenerator
        main.cpp
        Country.cpp Country.h
        MyVertex.cpp MyVertex.h
        MyArc.cpp MyArc.h
        MyRealisation.cpp MyRealisation.h
        Network.cpp Network.h
        Writer.cpp Writer.h
        BeeColony.cpp BeeColony.h
        Bee.cpp Bee.h
        Path.h Path.cpp
        Car.cpp Car.h
        DFS.cpp DFS.h
        EvolutionaryAlgorithm.cpp EvolutionaryAlgorithm.h)
