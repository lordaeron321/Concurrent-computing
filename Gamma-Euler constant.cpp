#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include <mutex>


double partialSum(int start, int end) {
    double sum = 0.0;
    for (int i = start; i <= end; ++i) {
        sum += 1.0 / i;
    }
    return sum;
}

void computePartialSum(int start, int end, double& result, std::mutex& mtx) {
    double partial = partialSum(start, end);
    std::lock_guard<std::mutex> lock(mtx);
    result += partial;
}

int main() {
    int n, p;
    std::cout << "number of batches (n): ";
    std::cin >> n;
    std::cout << "number of threads (p): ";
    std::cin >> p;

    std::vector<std::thread> threads;
    std::vector<double> partialResults(p, 0.0);
    std::mutex mtx;

    int batchSize = n / p;
    int rest = n % p;
    int start = 1;
    int end = batchSize;

    for (int i = 0; i < p; ++i) {
        if (i < rest) {
            end++;
        }
        threads.emplace_back(computePartialSum, start, end, std::ref(partialResults[i]), std::ref(mtx));
        start = end + 1;
        end = start + batchSize - 1;
    }

    for (auto& t : threads) {
        t.join();
    }

    double totalSum = 0.0;
    for (double val : partialResults) {
        totalSum += val;
    }

    totalSum -= log(n);

    std::cout << "approximation of Euler's constant: " << totalSum << std::endl;


    return 0;
}
