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
        std::sort(data.begin(), data.end());
        size_t n = data.size();
        return (n % 2 == 0) ? (data[n/2 - 1] + data[n/2]) / 2.0 : data[n/2];
    }

    static std::vector<double> getMode(const std::vector<double>& data) {
        if (data.empty()) return {};
        std::map<double, int> freq;
        for (double val : data) freq[val]++;
        int max_f = 0;
        for (auto const& [val, count] : freq) if (count > max_f) max_f = count;
        std::vector<double> modes;
        for (auto const& [val, count] : freq) if (count == max_f) modes.push_back(val);
        return modes;
    }

    static double getStandardDeviation(const std::vector<double>& data) {
        if (data.size() < 2) return 0;
        double mean = getMean(data);
        double sum = 0;
        for(double x : data) sum += (x - mean) * (x - mean);
        return std::sqrt(sum / data.size());
    }

    static long long nCr(int n, int r) {
        if (r < 0 || r > n) return 0;
        if (r == 0 || r == n) return 1;
        if (r > n / 2) r = n - r;
        long long res = 1;
        for (int i = 1; i <= r; ++i) res = res * (n - i + 1) / i;
        return res;
    }

    static long long nPr(int n, int r) {
        if (r < 0 || r > n) return 0;
        long long res = 1;
        for (int i = 0; i < r; ++i) res *= (n - i);
        return res;
    }

    static double binomialProb(int n, int k, double p) {
        if (p < 0.0 || p > 1.0 || k < 0 || k > n) return 0; 
        return (double)nCr(n, k) * std::pow(p, k) * std::pow(1.0 - p, n - k);
    }

    // --- Independent Event Helper ---
    // Figures out base P(A) and P(B) from whatever 2 values were given
    static void normalize(double& pa, double& pb, double pa_n, double pb_n, double inter) {
        if (pa == -1 && pa_n != -1) pa = 1.0 - pa_n;
        if (pb == -1 && pb_n != -1) pb = 1.0 - pb_n;
        if (pa != -1 && inter != -1 && pb == -1 && pa != 0) pb = inter / pa;
        if (pb != -1 && inter != -1 && pa == -1 && pb != 0) pa = inter / pb;
    }
};
#endif