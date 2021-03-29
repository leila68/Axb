cmake_minimum_required(VERSION 3.17)
project(Axb)

set(CMAKE_CXX_STANDARD 14)

add_executable(Axb main.cpp CSR.cpp TriangularSolve.cpp Matrix.cpp CSC.cpp CSC.h diagonal.cpp diagonal.h )