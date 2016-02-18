#include "normal_dist_pdf.h"

#define M_PI 3.14159

double normal_dist_pdf(const std::normal_distribution<double> & dist, const double & val) {
    const double mean = dist.mean();
    const double stddev = dist.stddev();
    const double prob = exp(-((val-mean)*(val-mean)/2*stddev*stddev)) / (stddev * sqrt(2*M_PI));
    return prob;
}
