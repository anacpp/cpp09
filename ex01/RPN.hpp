#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>

class RPN
{
public:
	RPN(void);
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN(void);

	int evaluate(const std::string &expression);

private:
	std::stack<int> _operands;

	static bool isOperator(const std::string &token);
	static int calculate(int left, int right, char operation);
};

#endif
