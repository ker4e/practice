#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>

std::once_flag flag;
void print_once()
{
    std::cout << "The number of hardware cores: " << std::thread::hardware_concurrency() << std::endl;
}

static std::vector<int> createVector(int x)
{
    std::vector<int> V(x);
    std::mt19937 gen;
    std::uniform_int_distribution<int> dis(0, x);
    auto rand_num([=]() mutable {return dis(gen); });
    std::generate(V.begin(), V.end(), rand_num);
    return V;
}

void addPart(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& c, int start, int end)
{
    std::call_once(flag, print_once);
    for (int i = start; i < end; ++i)
    {
        c[i] = a[i] + b[i];
    }
}

double addVectors(const std::vector<int>& a, const std::vector<int>& b, int threadCount)
{
    std::vector<int> c(a.size());
    std::vector<std::thread> threads;

    int size = static_cast<int>(a.size());
    int part = size / threadCount;

    auto startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threadCount; ++i)
    {
        int start = i * part;
        int end;
      
        if (i == threadCount - 1)
        {
            end = size;
        }
        else
        {
            end = start + part;
        }
        threads.push_back(std::thread(addPart, std::cref(a), std::cref(b), std::ref(c), start, end));
    }

    for (auto& thread : threads)
    {
        thread.join();
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = endTime - startTime;
    return time.count();
}

int bestResult(const std::vector<std::vector<double>>& results, int column)
{
    int best{};

    for (int i = 1; i < results.size(); ++i)
    {
        if (results[i][column] < results[best][column])
        {
            best = i;
        }
    }
    return best;
}

int main()
{

    std::vector<int> sizes{ 1'000, 10'000, 100'000, 1'000'000 };
    std::vector<int> threadCounts{ 1, 2, 4, 8, 16 };
    std::vector<std::vector<double>> results(threadCounts.size(), std::vector<double>(sizes.size()));

    for (int j = 0; j < sizes.size(); ++j)
    {
        std::vector<int> a = createVector(sizes[j]);
        std::vector<int> b = createVector(sizes[j]);

        for (int i = 0; i < threadCounts.size(); ++i)
        {
            results[i][j] = addVectors(a, b, threadCounts[i]);
        }
    }
  
    std::cout << "\nThreads\t";
    for (int size : sizes)
    {
        std::cout << size << "\t";
    }
  
    std::cout << '\n';
    for (int i = 0; i < threadCounts.size(); ++i)
    {
        std::cout << threadCounts[i] << "\t";
        for (int j = 0; j < sizes.size(); ++j)
        {
            std::cout << results[i][j] << "\t";
        }
        std::cout << '\n';
    }

    std::cout << std::endl;
    for (int j = 0; j < sizes.size(); ++j)
    {
        int best = bestResult(results, j);
        std::cout << sizes[j]
            << " elements - "
            << threadCounts[best]
            << " threads\n";
    }

    return EXIT_SUCCESS;
}
