#include <iostream>
#include <algorithm>
#include <future>
#include <vector>
#include <iterator>

template <typename Iterator, typename Func>
void parForEach(Iterator begin, Iterator end, Func& func)
{
	auto lenght = std::distance(begin, end);
	if (lenght <= 0)
		return;
	if (lenght <= 4)
	{
		std::for_each(begin, end, func);
		return;
	}

	Iterator middle = begin;
	std::advance(middle, lenght / 2);

	auto task1 = std::async(std::launch::async, [&]() {parForEach(begin, middle, func);});

	auto task2 = std::async(std::launch::async, [&]() {parForEach(middle, end, func);});

	task1.get();
	task2.get();
}

int main()
{
	std::vector<int> v{
		1, 2, 3, 4, 5,
		6, 7, 8, 9, 10,
		11, 12, 13, 14
	};

	auto func = [](int& value) {value *= 2;};
	parForEach(v.begin(), v.end(), func);

	for (int value : v)
	{
		std::cout << value << ' ';
	}

	return EXIT_SUCCESS;
}
