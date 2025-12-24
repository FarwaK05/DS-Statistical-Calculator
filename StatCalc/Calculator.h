#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <map>

class Calculator {
public:
    static double getMean(const std::vector<double>& data) {
        if (data.empty()) return 0;
        return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
    }

    static double getMedian(std::vector<double> data) {
        if (data.empty()) return 0;
        size_t n = data.size();
        if (n % 2 == 0) return (data[n/2 - 1] + data[n/2]) / 2;
        return data[n/2];
    }

    static double getStandardDeviation(const std::vector<double>& data) {
        if (data.size() < 2) return 0;
        double mean = getMean(data);
        double sq_sum = 0;
        for(double x : data) sq_sum += (x - mean) * (x - mean);
        return std::sqrt(sq_sum / data.size());
    }

    static long long nPr(int n, int r) {
        if (r > n) return 0;
        long long res = 1;
        for (int i = 0; i < r; i++) res *= (n - i);
        return res;
    }

    static long long nCr(int n, int r) {
        if (r > n) return 0;
        if (r == 0 || r == n) return 1;
        if (r > n / 2) r = n - r;
        long long res = 1;
        for (int i = 1; i <= r; ++i) {
            res = res * (n - i + 1) / i;
        }
        return res;
    }
};
#endif