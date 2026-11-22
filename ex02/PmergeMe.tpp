#ifndef PMERGEME_TPP
# define PMERGEME_TPP

# include <algorithm>
# include <iostream>

template <template <typename, typename> class Container>
void PmergeMe::insertPending(
	Container<Node, std::allocator<Node> > &chain,
	const Pending &pending)
{
	typedef Container<Node, std::allocator<Node> > NodeContainer;
	std::size_t bound = chain.size();
	std::size_t index;
	std::size_t low;
	std::size_t high;

	if (pending.hasPartner)
	{
		index = 0;
		while (index < chain.size() && chain[index].id != pending.partnerId)
			++index;
		bound = index;
	}
	low = 0;
	high = bound;
	while (low < high)
	{
		std::size_t middle = low + (high - low) / 2;
		if (chain[middle].value < pending.node.value)
			low = middle + 1;
		else
			high = middle;
	}
	chain.insert(chain.begin() + static_cast<typename NodeContainer::difference_type>(low),
		pending.node);
}

template <template <typename, typename> class Container>
void PmergeMe::fordJohnson(
	Container<Node, std::allocator<Node> > &sequence)
{
	typedef Container<Node, std::allocator<Node> > NodeContainer;
	typedef Container<Pair, std::allocator<Pair> > PairContainer;
	typedef Container<Pending, std::allocator<Pending> > PendingContainer;
	PairContainer pairs;
	NodeContainer largeElements;
	NodeContainer chain;
	PendingContainer pending;
	Node stray;
	bool hasStray = sequence.size() % 2 != 0;
	std::size_t index;

	if (sequence.size() < 2)
		return ;
	index = 0;
	while (index + 1 < sequence.size())
	{
		Pair pair;
		if (sequence[index].value <= sequence[index + 1].value)
		{
			pair.small = sequence[index];
			pair.large = sequence[index + 1];
		}
		else
		{
			pair.small = sequence[index + 1];
			pair.large = sequence[index];
		}
		pairs.push_back(pair);
		largeElements.push_back(pair.large);
		index += 2;
	}
	if (hasStray)
		stray = sequence[sequence.size() - 1];
	fordJohnson<Container>(largeElements);

	index = 0;
	while (index < pairs.size()
		&& pairs[index].large.id != largeElements[0].id)
		++index;
	chain.push_back(pairs[index].small);
	index = 0;
	while (index < largeElements.size())
	{
		chain.push_back(largeElements[index]);
		if (index > 0)
		{
			std::size_t pairIndex = 0;
			Pending item;
			while (pairs[pairIndex].large.id != largeElements[index].id)
				++pairIndex;
			item.node = pairs[pairIndex].small;
			item.partnerId = pairs[pairIndex].large.id;
			item.hasPartner = true;
			pending.push_back(item);
		}
		++index;
	}
	if (hasStray)
	{
		Pending item;
		item.node = stray;
		item.partnerId = 0;
		item.hasPartner = false;
		pending.push_back(item);
	}

	std::size_t totalB = pending.size() + 1;
	std::size_t previousBoundary = 1;
	std::size_t previousJacobsthal = 1;
	std::size_t jacobsthal = 3;
	while (previousBoundary < totalB)
	{
		std::size_t boundary = std::min(jacobsthal, totalB);
		std::size_t bIndex = boundary;
		while (bIndex > previousBoundary)
		{
			insertPending<Container>(chain, pending[bIndex - 2]);
			--bIndex;
		}
		previousBoundary = boundary;
		if (boundary == totalB)
			break ;
		std::size_t next = jacobsthal + 2 * previousJacobsthal;
		previousJacobsthal = jacobsthal;
		jacobsthal = next;
	}
	sequence = chain;
}

template <template <typename, typename> class Container>
void PmergeMe::sortContainer(Container<int, std::allocator<int> > &numbers)
{
	Container<Node, std::allocator<Node> > nodes;
	std::size_t index = 0;

	while (index < numbers.size())
	{
		nodes.push_back(Node(numbers[index], index));
		++index;
	}
	fordJohnson<Container>(nodes);
	index = 0;
	while (index < nodes.size())
	{
		numbers[index] = nodes[index].value;
		++index;
	}
}

template <typename Container>
void PmergeMe::printSequence(const Container &numbers)
{
	typename Container::const_iterator iterator = numbers.begin();

	while (iterator != numbers.end())
	{
		std::cout << " " << *iterator;
		++iterator;
	}
	std::cout << std::endl;
}

#endif
