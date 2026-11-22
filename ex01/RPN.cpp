#include "RPN.hpp"

#include <limits>
#include <sstream>
#include <stdexcept>

RPN::RPN(void)
{
}

RPN::RPN(const RPN &other)
	: _operands(other._operands)
{
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		_operands = other._operands;
	return (*this);
}

RPN::~RPN(void)
{
}

bool RPN::isOperator(const std::string &token)
{
	return (token.size() == 1
		&& (token[0] == '+' || token[0] == '-' || token[0] == '*'
			|| token[0] == '/'));
}

int RPN::calculate(int left, int right, char operation)
{
	long result;

	if (operation == '/' && right == 0)
		throw std::runtime_error("division by zero");
	if (operation == '+')
		result = static_cast<long>(left) + right;
	else if (operation == '-')
		result = static_cast<long>(left) - right;
	else if (operation == '*')
		result = static_cast<long>(left) * right;
	else
		result = left / right;
	if (result > std::numeric_limits<int>::max()
		|| result < std::numeric_limits<int>::min())
		throw std::runtime_error("integer overflow");
	return (static_cast<int>(result));
}

int RPN::evaluate(const std::string &expression)
{
	std::istringstream stream(expression);
	std::string token;

	_operands = std::stack<int>();
	while (stream >> token)
	{
		if (token.size() == 1 && token[0] >= '0' && token[0] <= '9')
			_operands.push(token[0] - '0');
		else if (isOperator(token))
		{
			int right;
			int left;

			if (_operands.size() < 2)
				throw std::runtime_error("invalid expression");
			right = _operands.top();
			_operands.pop();
			left = _operands.top();
			_operands.pop();
			_operands.push(calculate(left, right, token[0]));
		}
		else
			throw std::runtime_error("invalid token");
	}
	if (_operands.size() != 1)
		throw std::runtime_error("invalid expression");
	return (_operands.top());
}
