#include "BitcoinExchange.hpp"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error: usage: ./btc <input_file>" << std::endl;
		return (1);
	}
	try
	{
		std::string executable(argv[0]);
		std::string databasePath("data.csv");
		std::string::size_type slash = executable.find_last_of('/');
		if (slash != std::string::npos)
			databasePath = executable.substr(0, slash + 1) + "data.csv";
		BitcoinExchange exchange(databasePath);
		exchange.processFile(argv[1]);
	}
	catch (const std::exception &exception)
	{
		std::cerr << "Error: " << exception.what() << std::endl;
		return (1);
	}
	return (0);
}
