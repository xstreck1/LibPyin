#ifndef NORMAL_DIST_PDF_H
#define NORMAL_DIST_PDF_H

#include <random>

// given the normal distribution dist computes the probablity of the element val in that distribution
double normal_dist_pdf(const std::normal_distribution<double> & dist, const double & val);

#endif // NORMAL_DIST_PDF_H
