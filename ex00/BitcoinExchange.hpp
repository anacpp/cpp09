#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

class BitcoinExchange
{
public:
	BitcoinExchange(const std::string &databasePath);
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange(void);

	void processFile(const std::string &inputPath) const;

private:
	BitcoinExchange(void);

	std::map<std::string, double> _rates;

	static std::string trim(const std::string &text);
	static bool isValidDate(const std::string &date);
	static bool parseNumber(const std::string &text, double &number);
	double rateFor(const std::string &date) const;
};

#endif
