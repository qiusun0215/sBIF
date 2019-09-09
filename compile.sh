#!/bin/bash
g++ --std=c++11 -fopenmp check.cpp loading.cpp dumping.cpp optimization.cpp parsingargs.cpp main.cpp -o sbif
