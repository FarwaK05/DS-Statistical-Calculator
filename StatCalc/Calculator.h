#ifndef CALCULATOR_H
#define CALCULATOR_H
#include <vector>
#include <numeric>
#include <cmath>
#include <algorithm>

class Calculator {
private:
    static double round3(double val) {
        return std::round(val * 1000.0) / 1000.0;
    }

public:
    static double getMean(const std::vector<double>& data) {
        if (data.empty()) return 0.0;
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        return round3(sum / data.size());
    }

    static double getMedian(std::vector<double> data) {
        if (data.empty()) return 0.0;
        std::sort(data.begin(), data.end());
        size_t n = data.size();
        double median = (n % 2 == 0) ? (data[n/2 - 1] + data[n/2]) / 2.0 : data[n/2];
        return round3(median);
    }

    static std::vector<double> getMode(const std::vector<double>& data) {
        if (data.empty()) return {};
        
        int maxCount = 0;
        int currentCount = 0;

        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0 && data[i] == data[i - 1]){
                currentCount++;
            } 
            else currentCount = 1;
            if (currentCount > maxCount) {
                maxCount = currentCount;
            }
        }
        std::vector<double> modes;
        currentCount = 0;
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0 && data[i] == data[i - 1]) {
                currentCount++;}
            else currentCount = 1;
            if (currentCount == maxCount) {
                modes.push_back(round3(data[i]));
            }
        }
        return modes;
    }

    static double getStandardDeviation(const std::vector<double>& data) {
        if (data.size() < 2) return 0.0;
        double mean = std::accumulate(data.begin(), data.end(), 0.0) / data.size();
        double sumSq = 0;
        for(double x : data) sumSq += (x - mean) * (x - mean);
        return round3(std::sqrt(sumSq / data.size()));
    }

    static double nCr(int n, int r) {
        if (n < 0 || r < 0 || r > n) return 0.0; 
        if (r == 0 || r == n) return 1.000;
        if (r > n / 2) r = n - r;
        double res = 1;
        for (int i = 1; i <= r; ++i) {
            res = res * (n - i + 1) / i;
        }
        return round3(res);
    }

    static double nPr(int n, int r) {
        if (n < 0 || r < 0 || r > n) return 0.0;
        double res = 1;
        for (int i = 0; i < r; ++i) {
            res *= (n - i);
        }
        return round3(res);
    }

    static double binomialProb(int n, int k, double p) {
        if (n < 0 || k < 0 || k > n || p < 0.0 || p > 1.0) return 0.0; 
        double result = (double)nCr(n, k) * std::pow(p, k) * std::pow(1.0 - p, n - k);
        return round3(result);
    }

    static void normalize(double& pa, double& pb, double pa_n, double pb_n, double inter) {
        if (pa == -1 && pa_n != -1) {
            pa = 1.0 - pa_n;}
        if (pb == -1 && pb_n != -1) {
            pb = 1.0 - pb_n;}
        if (pa != -1 && inter != -1 && pb == -1 && pa > 0) {
            pb = inter / pa;}
        if (pb != -1 && inter != -1 && pa == -1 && pb > 0) {
            pa = inter / pb;}
        if (pa != -1) {
            pa = round3(pa);}
        if (pb != -1) {
            pb = round3(pb);}
    }
};
#endif