#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std::chrono_literals;
std::atomic<int> queue = 0;

void client(std::atomic<int>& x, int maxClients)
{
    for (auto i{ 0 }; i < maxClients; ++i)
    {
        std::this_thread::sleep_for(1s);
        
        x.fetch_add(1, std::memory_order_seq_cst);
        std::cout << "Clients in queue: " << x.load(std::memory_order_seq_cst) << std::endl;
    }
}

void cashier(std::atomic<int>& x, int maxClients)
{
    int served{ 0 };

    while (served < maxClients)
    {
        std::this_thread::sleep_for(2s);
        
        if (x.load(std::memory_order_seq_cst) > 0)
        {
            x.fetch_sub(1, std::memory_order_seq_cst);
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
