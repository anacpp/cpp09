#include "BitcoinExchange.hpp"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

BitcoinExchange::BitcoinExchange(const std::string &databasePath)
{
	std::ifstream database(databasePath.c_str());
	std::string line;

	if (!database)
		throw std::runtime_error("could not open database");
	if (!std::getline(database, line) || trim(line) != "date,exchange_rate")
		throw std::runtime_error("invalid database header");
	while (std::getline(database, line))
	{
		std::string::size_type delimiter = line.find(',');
		std::string date;
		double rate;

		if (delimiter == std::string::npos)
			throw std::runtime_error("invalid database line");
		date = trim(line.substr(0, delimiter));
		if (!isValidDate(date)
			|| !parseNumber(trim(line.substr(delimiter + 1)), rate) || rate < 0.0)
			throw std::runtime_error("invalid database line");
		_rates[date] = rate;
	}
	if (_rates.empty())
		throw std::runtime_error("empty database");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: _rates(other._rates)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_rates = other._rates;
	return (*this);
}

BitcoinExchange::~BitcoinExchange(void)
{
}

std::string BitcoinExchange::trim(const std::string &text)
{
	std::string::size_type first = 0;
	std::string::size_type last = text.size();

	while (first < last && std::isspace(static_cast<unsigned char>(text[first])))
		++first;
	while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1])))
		--last;
	return (text.substr(first, last - first));
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
	static const int daysPerMonth[] = {31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31};
	int year;
	int month;
	int day;
	int maximumDay;
	std::string::size_type index;

	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	index = 0;
	while (index < date.size())
	{
		if (index != 4 && index != 7
			&& !std::isdigit(static_cast<unsigned char>(date[index])))
			return (false);
		++index;
	}
	year = (date[0] - '0') * 1000 + (date[1] - '0') * 100
		+ (date[2] - '0') * 10 + date[3] - '0';
	month = (date[5] - '0') * 10 + date[6] - '0';
	day = (date[8] - '0') * 10 + date[9] - '0';
	if (year == 0 || month < 1 || month > 12)
		return (false);
	maximumDay = daysPerMonth[month - 1];
	if (month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
		maximumDay = 29;
	return (day >= 1 && day <= maximumDay);
}

bool BitcoinExchange::parseNumber(const std::string &text, double &number)
{
	std::istringstream stream(text);
	char extra;

	if (text.empty() || !(stream >> number) || (stream >> extra))
		return (false);
	if (number != number || number > std::numeric_limits<double>::max()
		|| number < -std::numeric_limits<double>::max())
		return (false);
	return (true);
}

double BitcoinExchange::rateFor(const std::string &date) const
{
	std::map<std::string, double>::const_iterator rate = _rates.lower_bound(date);

	if (rate != _rates.end() && rate->first == date)
		return (rate->second);
	if (rate == _rates.begin())
		throw std::runtime_error("no earlier exchange rate available");
	--rate;
	return (rate->second);
}

void BitcoinExchange::processFile(const std::string &inputPath) const
{
	std::ifstream input(inputPath.c_str());
	std::string line;

	if (!input)
		throw std::runtime_error("could not open input file");
	if (!std::getline(input, line) || trim(line) != "date | value")
		throw std::runtime_error("invalid input header");
	while (std::getline(input, line))
	{
		std::string::size_type delimiter = line.find('|');
		std::string date;
		double value;

		if (delimiter == std::string::npos
			|| line.find('|', delimiter + 1) != std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		date = trim(line.substr(0, delimiter));
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (!parseNumber(trim(line.substr(delimiter + 1)), value))
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		if (value < 0.0)
		{
			std::cerr << "Error: not a positive number." << std::endl;
			continue;
		}
		if (value > 1000.0)
		{
			std::cerr << "Error: too large a number." << std::endl;
			continue;
		}
		try
		{
			double result = value * rateFor(date);
			std::cout << date << " => " << value << " = "
				<< result << std::endl;
		}
		catch (const std::exception &exception)
		{
			std::cerr << "Error: " << exception.what() << std::endl;
		}
	}
}
