#include <iostream>
#include <mutex>

class Data
{
public:
    int value;
    std::mutex m;

    Data(int value)
    {
        this->value = value;
    }
};

void swap_lock(Data& a, Data& b)
{
    std::lock(a.m, b.m);
    std::lock_guard<std::mutex> lock1(a.m, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(b.m, std::adopt_lock);
    std::swap(a.value, b.value);
}


void swap_scoped(Data& a, Data& b)
{
    std::scoped_lock lock(a.m, b.m);
    std::swap(a.value, b.value);
}

void swap_unique(Data& a, Data& b)
{
    std::unique_lock<std::mutex> lock1(a.m, std::defer_lock);
    std::unique_lock<std::mutex> lock2(b.m, std::defer_lock);
    std::lock(lock1, lock2);
    std::swap(a.value, b.value);
}

void task3(Data& a, Data& b)
{
    std::cout << a.value << " " << b.value << std::endl;

    swap_lock(a, b);
    std::cout << a.value << " " << b.value << std::endl;

    swap_scoped(a, b);
    std::cout << a.value << " " << b.value << std::endl;

    swap_unique(a, b);
    std::cout << a.value << " " << b.value << std::endl;
}

int main()
{
    Data a(10);
    Data b(20);
    task3(a, b);

    return 0;
}
