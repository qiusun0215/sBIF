#!/bin/bash
g++ --std=c++11 -fopenmp check.cpp loading.cpp dumping.cpp MC.cpp main.cpp -o sis-mc-time
