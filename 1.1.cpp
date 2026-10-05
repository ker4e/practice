#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std::chrono_literals;
int queue = 0;
std::mutex m;


void client(int& x, int maxClients)
{
    for (auto i{ 0 }; i < maxClients; ++i)
    {
        std::this_thread::sleep_for(1s);
        std::lock_guard<std::mutex> lock(m);
        ++x;
        std::cout << "Clients in queue: " << x << std::endl;
        
    }
}

void cashier(int& x, int maxClients)
{
    int served{ 0 };
    
    while (served < maxClients)
    {
        std::this_thread::sleep_for(2s);
        std::lock_guard<std::mutex> lock(m);
        if (x > 0)
        {
            --x;
            ++served;
            std::cout << "Served: " << served << std::endl;
        }
    }
}


int main()
{
    int maxClients{ 10 };
    std::thread t1(client, std::ref(queue), maxClients);
    std::thread t2(cashier, std::ref(queue), maxClients);

    t1.join();
    t2.join();

    return 0;
}
