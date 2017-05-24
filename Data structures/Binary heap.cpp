/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Binary heap data structure. Used as a priority queue. 
			 Always gives the biggest element in the set, uses < operator only.
			 Space complexity: O(n).
			 Operations:
				 * Push - insert new element. Time complexity: O(log n).
				 * Pop - removes biggest element from the set. Time complexity: O(log n).
				 * Top - returns biggest element in the sett. Time complexity O(1).
				 * Size - returns the size of the heap (the number of elements in. Time complexity: O(1).
				 * Capacity - returns the capacity of the heap vector - does not have to be same as size. Time complexity: O(1).
				 * MaxSize - returns maximum capacity of the heap. Time complexity: O(1).
				 * Empty - returns true if there is no element in heap. Time complexity: O(1).
Notes:
	  * It uses std::vector as a container, to provide auto extension of the container if needed.
	  * Only operator that has to be implemented is '<'. Every comparation is used only with this operator.
	  * In most cases it works much faster than std::proprity_queue.
===============================================================================*/

#include <iostream>
#include <vector>

template <class T> 
class BinaryHeap
{
private:
	// The main vector used as a heap.
	// Note that first node has index of 1 to provide easer navigation so heap[0] is never used.
	std::vector < T > heap;

	// Inline functions used to get indexs of the parent and childs of the vertex.
	inline int Parent(int v) { return v / 2; }
	inline int LeftChild(int v) { return v * 2; }
	inline int RightChild(int v) { return v * 2 + 1; }

	// Inline funcions used for comparing objects. Only < operator is used for comparation.
	inline bool Lower(T self, T other) { return self < other; }
	inline bool Equal(T self, T other) { return (!(self < other) && !(other < self)); }
	inline bool Greater(T self, T other) { return (!Equal(self, other) && !(self < other)); }

public:
	// Returns the size of the heap, which is the number of elements currently in the container.
	int Size() { return heap.size() - 1; }

	// Returns the capacity of the heap.
	int Capacity() { return heap.capacity(); } 

	//Returns the max size of the heap.
	long long MaxSize() { return heap.max_size(); }
	
	// Returns true if there is no emelent in the heap.
	bool Empty() { return Size() == 0; }

	// Add an element to the set.
	void Push(T item)
	{
		// Push new item to the end of the heap vector.
		heap.push_back(item);

		// Now move it upwards until its parent is greather than it, or it is on the top of the heap.
		// Starting from the last element in the vector...
		int curren_idx = heap.size() - 1;\

		// While it is not on the top...
		while (curren_idx > 1)
		{
			// Compare it with its parent;
			// If greater swap elements and update current index of the inserted node.
			if (Greater(heap[curren_idx], heap[Parent(curren_idx)]))
			{
				std::swap(heap[curren_idx], heap[Parent(curren_idx)]);
				curren_idx = Parent(curren_idx);
			}
			// Ohterwise break the while loop.
			else
				break;
		}

	}

	// Return biggest element in the heap, which is always on its top.
	T Top() { return heap[1]; }

	// Remove biggest element from the set.
	void Pop()
	{
		// Move last element to the top. 
		std::swap(heap[1], heap.back());
		// Decrease number of elements.
		heap.pop_back();
		// If there are still elements in the set....
		if (!Empty())
			// Make sure that the heap rule is restricted and heapify on the top.
			Heapify(1);
	}

	// Default constructor.
	BinaryHeap()
	{
		heap = std::vector<T>(1);
	}

	// Constructor used if size is known before adding values to the heap.
	BinaryHeap(int size)
	{
		heap = std::vector<T>(size+1);
	}
	
	// Constructor used to create a heap from an existing array by giving a pointer to the array and its size.
	BinaryHeap(T *elements, int size)
	{
		heap = std::vector<T>(size + 1);
		for (int i = 0; i < size; ++i)
			heap[i + 1] = elements[i];
		for (std::size_t i = heap.size() - 1; i >= 1; --i)
			Heapify(i);
	}

	// Constructor used to create a heap from an existing vector.
	BinaryHeap(std::vector < T> elements)
	{
		heap = std::vector<T>(elements.size() + 1);
		for (int i = 0; i < elements.size(); ++i)
			heap[i + 1] = elements[i];
	}

	// Copy constructor.
	BinaryHeap(const BinaryHeap<T> &other)
	{
		heap = std::vector <T> (other.heap.size());
	}
	
private:
	// For given vertex v if the heap condition is fullfield value in v is greater than child values, 
	// or if v is a leaf node, function returns.
	// Otherwise values are swapped and function is called recursively in a lower nodes.
	void Heapify(int v)
	{
		// If left child doesn't exist (right one does not as well) so it is a leaf node, so function returns.
		if (LeftChild(v) >= heap.size())
			return;

		// Handle a case when only left child exists.
		// No further function calls required because left child is a leaf node in this case.
		if (RightChild(v) >= heap.size())
		{
			if (Lower(heap[v], heap[LeftChild(v)]))
				std::swap(heap[v], heap[LeftChild(v)]);
			return;
		}

		// In the other case take greater of both childs and compare to the current node.
		// If current node is less than the greater one swap and call recursively in one of the childs.
		int greater_leaf_index = LeftChild(v);
		if (Greater(heap[RightChild(v)], heap[LeftChild(v)]))
			greater_leaf_index = RightChild(v);
		if (Lower(heap[v], heap[greater_leaf_index]))
		{
			std::swap(heap[v], heap[greater_leaf_index]);
			Heapify(greater_leaf_index);
		}
	}
};

int main()
{
	int tab[] = { 43, 1, 6, 3 , 41, 51, 36, 23, 11};
	BinaryHeap<int> *bh = new BinaryHeap<int>(tab, 9);

	std::cout << "Building heap over the 9 element array ( ";
	for (int i = 0; i < 9; ++i)
		std::cout << tab[i] << " ";
	std::cout << ")\n";
	std::cout << "Biggest value is " << bh->Top() << "\n";
	bh->Pop();
	std::cout << "Removing biggest value...\n";
	std::cout << "Now biggest value is: " << bh->Top() << "\n";
	std::cout << "Adding 50 to the heap...\n";
	bh->Push(50);
	std::cout << "Rest of the values in the order as poped from the heap:\n";
	while (!bh->Empty())
	{
		std::cout << bh->Top() << " ";
		bh->Pop();
	}
	std::cout << "\n";
	//Output:
	//	Building heap over the 9 element array(43 1 6 3 41 51 36 23 11)
	//	Biggest value is 51
	//	Removing biggest value...
	//	Now biggest value is : 43
	//	Adding 50 to the heap...
	//	Rest of the values in the order as poped from the heap :
	//	50 43 41 36 23 11 6 3 1
	
	return 0;
}