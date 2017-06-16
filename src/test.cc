#include <future>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	try
	{
		auto test_lambda              = [] (int a, int b, int c) -> void { std::cout << "a: " << a << " b: " << b << " c: " << c << "." << std::endl; };
		std::future<void> test_future = std::async(test_lambda, 1, 2, 3);
		std::chrono::milliseconds milliseconds_100(100);
		while(test_future.wait_for(milliseconds_100) == std::future_status::timeout)
		{
			std::cout << "." << std::endl;
		}
	}
	catch(const std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}
}
