#include <iostream>
#include <vector>
#include <future>
#include <utility>

void findMin(const std::vector<int>& v, int start, std::promise<int> promise)
{
	int minIndex{ start };
	for (int i{ start + 1 }; i < v.size(); ++i)
	{
		if (v[i] < v[minIndex])
		{
			minIndex = i;
		}
	}
	promise.set_value(minIndex);
}

void selectSort(std::vector<int>& v)
{
	for (std::size_t i{ 0 }; i < v.size(); ++i)
	{
		std::promise<int> promise;
		std::future<int> future = promise.get_future();

		auto task = std::async(std::launch::async, findMin, std::cref(v), i, std::move(promise));

		int minIndex = future.get();
		std::swap(v[i], v[minIndex]);
	}
}

int main() 
{
	std::vector<int> v{ 8,4,5,3,2,5,7,0,9 };
	selectSort(v);

	for (int value : v)
	{
		std::cout << value << " ";
	}
	return EXIT_SUCCESS;
}
