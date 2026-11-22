#include "PmergeMe.hpp"

#include <climits>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

PmergeMe::Node::Node(void)
	: value(0), id(0)
{
}

PmergeMe::Node::Node(int number, std::size_t identifier)
	: value(number), id(identifier)
{
}

PmergeMe::PmergeMe(int argc, char **argv)
{
	int index = 1;

	while (index < argc)
	{
		int number = parsePositiveInteger(argv[index]);
		_vector.push_back(number);
		_deque.push_back(number);
		++index;
	}
}

PmergeMe::PmergeMe(const PmergeMe &other)
	: _vector(other._vector), _deque(other._deque)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vector = other._vector;
		_deque = other._deque;
	}
	return (*this);
}

PmergeMe::~PmergeMe(void)
{
}

int PmergeMe::parsePositiveInteger(const char *text)
{
	std::string value(text);
	std::string::size_type index = 0;
	long number;
	char extra;
	std::istringstream stream(value);

	if (value.empty())
		throw std::runtime_error("invalid positive integer");
	while (index < value.size())
	{
		if (value[index] < '0' || value[index] > '9')
			throw std::runtime_error("invalid positive integer");
		++index;
	}
	if (!(stream >> number) || (stream >> extra) || number <= 0 || number > INT_MAX)
		throw std::runtime_error("invalid positive integer");
	return (static_cast<int>(number));
}

void PmergeMe::run(void)
{
	std::vector<int> original = _vector;
	std::clock_t vectorStart;
	std::clock_t vectorEnd;
	std::clock_t dequeStart;
	std::clock_t dequeEnd;

	std::cout << "Before:";
	printSequence(original);
	vectorStart = std::clock();
	sortContainer<std::vector>(_vector);
	vectorEnd = std::clock();
	dequeStart = std::clock();
	sortContainer<std::deque>(_deque);
	dequeEnd = std::clock();
	std::cout << "After:";
	printSequence(_vector);
	std::cout << std::fixed << std::setprecision(3);
	std::cout << "Time to process a range of " << _vector.size()
		<< " elements with std::vector : "
		<< static_cast<double>(vectorEnd - vectorStart) * 1000000.0
			/ CLOCKS_PER_SEC << " us" << std::endl;
	std::cout << "Time to process a range of " << _deque.size()
		<< " elements with std::deque  : "
		<< static_cast<double>(dequeEnd - dequeStart) * 1000000.0
			/ CLOCKS_PER_SEC << " us" << std::endl;
}
