#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <Windows.h>

namespace console_parameter
{
    void SetPosition(short x, short y)
    {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD position;
        position.X = x;
        position.Y = y;
        SetConsoleCursorPosition(hConsole, position);
    }
}


std::mutex m;

void drawRowProgress(int row, int calculationLength)
{
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < calculationLength; ++i)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::lock_guard<std::mutex> lock(m);

        // Каждый поток рисует только в своей строке
        console_parameter::SetPosition(0, row);
        std::cout << row + 1 << "  " << std::this_thread::get_id() << "  [";

        for (int j = 0; j < calculationLength; ++j)
        {
            if (j <= i)
                std::cout << '#';
            else
                std::cout << ' ';
        }
        std::cout << "]" << std::flush;
    }

    auto end = std::chrono::steady_clock::now();
    double time = std::chrono::duration<double>(end - start).count();
    {
        std::lock_guard<std::mutex> lock(m);
        console_parameter::SetPosition(calculationLength + 30, row);
        std::cout << " " << time << " sec";
    }
}

void task2()
{
    int threadCount = 5;
    int calculationLength = 20;

    std::thread* t = new std::thread[threadCount];

    for (size_t i = 0; i < threadCount; ++i)
    {
        t[i] = std::thread(drawRowProgress, i, calculationLength);
    }

    for (size_t i = 0; i < threadCount; ++i)
    {
        t[i].join();
    }
    console_parameter::SetPosition(0, threadCount + 1);
    delete[] t;
}

int main()
{
    try {
        task2();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
