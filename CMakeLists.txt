cmake_minimum_required(VERSION 3.31)
project(main)

set(CMAKE_CXX_STANDARD 20)

add_executable(main main.cpp
        Solver.cpp
        Solver.h
        LevenshteinSolver.cpp
        LevenshteinSolver.h
        DamerauLevenshteinSolver.cpp
        DamerauLevenshteinSolver.h
        TrigramSolver.cpp
        TrigramSolver.h
        NoisyChannelSolver.cpp
        NoisyChannelSolver.h)
