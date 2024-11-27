#include "pch.h"
using namespace std;

std::vector<double> compute_sequence(double r, double x0, int steps) {
    std::vector<double> sequence(steps + 1);
    sequence[0] = x0;
    for (int n = 0; n < steps; ++n) {
        sequence[n + 1] = r * sequence[n] * (1 - sequence[n]);
    }
    return sequence;
}
