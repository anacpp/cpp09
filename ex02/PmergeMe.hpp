#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <cstddef>
# include <deque>
# include <memory>
# include <vector>

class PmergeMe
{
public:
	PmergeMe(int argc, char **argv);
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe(void);

	void run(void);

private:
	PmergeMe(void);

	struct Node
	{
		int value;
		std::size_t id;

		Node(void);
		Node(int number, std::size_t identifier);
	};

	struct Pair
	{
		Node small;
		Node large;
	};

	struct Pending
	{
		Node node;
		std::size_t partnerId;
		bool hasPartner;
	};

	std::vector<int> _vector;
	std::deque<int> _deque;

	static int parsePositiveInteger(const char *text);

	template <template <typename, typename> class Container>
	static void fordJohnson(
		Container<Node, std::allocator<Node> > &sequence);

	template <template <typename, typename> class Container>
	static void insertPending(
		Container<Node, std::allocator<Node> > &chain,
		const Pending &pending);

	template <template <typename, typename> class Container>
	static void sortContainer(Container<int, std::allocator<int> > &numbers);

	template <typename Container>
	static void printSequence(const Container &numbers);
};

# include "PmergeMe.tpp"

#endif
