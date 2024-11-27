#ifndef PCH_H
#define PCH_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <gtest/gtest.h>

using namespace std;

std::vector<double> compute_sequence(double r, double x0, int steps);

#endif // PCH_H
