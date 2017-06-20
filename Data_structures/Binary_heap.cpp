/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Binary heap data structure. Used as a priority queue.
             Always gives the greatest element in the set, uses < operator only.
             Space complexity: O(n).
Operations:
	  * Push - insert new element. Time complexity: O(log(n)).
	  * Pop - removes greatest element from the set. Time complexity: O(log(n)).
	  * Top - returns greatest element in the sett. Time complexity O(1).
	  * Size - returns the size of the heap (the number of elements in). Time complexity: O(1).
	  * Capacity - returns the capacity of the heap vector - does not have to be same as size. Time complexity: O(1).
	  * MaxSize - returns maximum capacity of the heap. Time complexity: O(1).
	  * Type - returns the type of the heap (maxHeap / minHeap). Time complexity: O(1).
	  * Empty - returns true if there is no element in heap. Time complexity: O(1).
	  * ChangeType - changes the type of the heap (form maxHeap to minHeap or vice versa) and rebuilds it. Time complexity: O(n*log(n)).
Notes:
	  * It uses std::vector as a container, to provide auto extension of the container if needed.
	  * Only operator that has to be implemented is '<'. Every comparation is used only with this operator.
	  * In most cases it works much faster than std::proprity_queue.
===============================================================================*/

#include <iostream>
#include <vector>
#include "..\Utility.cpp"

// Enum used to determine heap type. maxHeap is a heap with greatest element on the top, and minHeap
// is a heap with lowest element on the top.
enum HeapType
{
	maxHeap,
	minHeap
};

template <class T> 
class BinaryHeap
{
private:
	// The main vector used as a heap.
	// Note that first node has index of 1 to provide easer navigation so heap[0] is never used.
	std::vector < T > heap;

	// The type of the heap.
	HeapType heapType;

	// Functions used to get indexs of the parent and childs of the vertex.
	int Parent(int v) { return v / 2; }
	int LeftChild(int v) { return v * 2; }
	int RightChild(int v) { return v * 2 + 1; }

	// Function used by the heap operations used for comparing. Return value depends on the selected heap type.
	// True is first element has higher priority over the second one.
	bool HigherPriority(T self, T other)
	{
		if (heapType == maxHeap)
			return utility::IsGreater(self, other);
		else if (heapType == minHeap)
			return utility::IsLower(self, other);
		// This should never happen unless more HeapTypes are addded
		else
		{
			std::cerr << "Unrecognized heap type.\n";
			return false;
		}
	}

public:
	// Returns the size of the heap, which is the number of elements currently in the container.
	std::size_t Size() { return heap.size() - 1; }

	// Returns the capacity of the heap.
	int Capacity() { return heap.capacity(); } 

	// Returns the max size of the heap.
	long long MaxSize() { return heap.max_size(); }
	
	// Returns the type of the heap.
	HeapType Type() { return heapType; }
	
	// Returns true if there is no emelent in the heap.
	bool Empty() { return Size() == 0; }

	// Change the type of the heap and rebuild it. This function is the slowest in the data strucutre, 
	// as its builds the whole heap again. Not recommended to use often.
	void ChangeType(HeapType newType)
	{
		// If requested type is an old one do nothing.
		if (newType == heapType)
			return;

		// Change the type of the heap.
		heapType = newType;
		
		// This is not necessary, but allows the function to work faster in most cases. 
		// If removed function will work identically, but possible slower in some cases.
		std::reverse(heap.begin() + 1, heap.end());

		BuildHeap();
	}

	// Add an element to the set.
	void Push(T item)
	{
		// Push new item to the end of the heap vector.
		heap.push_back(item);

		// Now move it upwards until its parent has higher priority than it, or it is on the top of the heap.
		// Starting from the last element in the vector...
		int current_idx = (int)heap.size() - 1;

		// While it is not on the top...
		while (current_idx > 1)
		{
			// Compare it with its parent;
			// If it has higher priority over its parent, 
			// swap elements and update current index of the inserted node.
			if (HigherPriority(heap[current_idx], heap[Parent(current_idx)]))
			{
				std::swap(heap[current_idx], heap[Parent(current_idx)]);
				current_idx = Parent(current_idx);
			}
			// Ohterwise break the while loop.
			else
				break;
		}

	}

	// Return element with the highest priority in the heap, which is always on its top.
	T Top() { return heap[1]; }

	// Remove element with the highest priority from the set.
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
	BinaryHeap(HeapType type)
	{
		heapType = type;
		heap = std::vector<T>(1);
	}
	
	// Constructor used to create a heap from an existing array by giving a pointer to the array and its size.
	BinaryHeap(T *elements, int size, HeapType type)
	{
		heapType = type;
		heap = std::vector<T>(size + 1);
		for (int i = 0; i < size; ++i)
			heap[i + 1] = elements[i];
		BuildHeap();
	}

	// Constructor used to create a heap from an existing vector.
	BinaryHeap(std::vector < T> elements, HeapType type)
	{
		heapType = type;
		heap = std::vector<T>(elements.size() + 1);
		for (int i = 0; i < elements.size(); ++i)
			heap[i + 1] = elements[i];
		BuildHeap();
	}

	// Copy constructor.
	BinaryHeap(const BinaryHeap<T> &other)
	{
		heapType = other.heapType;
		heap = std::vector <T> (other.heap.size());
		for (int i = 0; i < heap.size(); ++i)
			heap[i] = other.heap[i];
	}
	
private:
	// For given vertex v if the heap condition is fullfield (value in v has higher priority than its child values), 
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
			if (HigherPriority(heap[LeftChild(v)], heap[v]))
				std::swap(heap[v], heap[LeftChild(v)]);
			return;
		}

		// In the other case take greater of both childs and compare to the current node.
		// If current node is less than the greater one swap and call recursively in one of the childs.
		int greater_leaf_index = LeftChild(v);
		if (HigherPriority(heap[RightChild(v)], heap[LeftChild(v)]))
			greater_leaf_index = RightChild(v);
		if (HigherPriority(heap[greater_leaf_index], heap[v]))
		{
			std::swap(heap[v], heap[greater_leaf_index]);
			Heapify(greater_leaf_index);
		}
	}

	// Iterate over heap nodes and set nodes that the heap rule is restricted.
	void BuildHeap()
	{
		for (int i = (int)heap.size() - 1; i >= 1; --i)
			Heapify(i);
	}
};

/*
//============== USAGE EXAMPLE ===================

int main()
{
	int arr[] = { 43, 1, 6, 3 , 41, 51, 36, 23, 11};
	BinaryHeap<int> *bh = new BinaryHeap<int>(arr, 9, maxHeap);
	std::cout << "Building max heap over the 9 element array ( ";
	for (int i = 0; i < 9; ++i)
		std::cout << arr[i] << " ";
	std::cout << ")\n";
	std::cout << "Greatest value is " << bh->Top() << "\n";
	bh->Pop();
	std::cout << "Removing greatest value...\n";
	std::cout << "Now greatest value is: " << bh->Top() << "\n";
	std::cout << "Adding 50 to the heap...\n";
	bh->Push(50);
	std::cout << "Rest of the values in the order as poped from the heap:\n";
	while (!bh->Empty())
	{
		std::cout << bh->Top() << " ";
		bh->Pop();
	}
	std::cout << "\n";
	std::cout << "Now building a min heap over the same array.\n";
	bh = new BinaryHeap<int>(arr, 9, minHeap);
	for (int i = 0; i < 3; ++i)
	{
		std::cout << "Lowest value is " << bh->Top() << "\n";
		std::cout << "Removing lowest value...\n";
		bh->Pop();
	}
	std::cout << "Change heap type to max heap again...\n";
	bh->ChangeType(maxHeap);
	std::cout << "Adding 25 to the heap...\n";
	bh->Push(25);
	std::cout << "Rest of the values in the order as poped from the heap:\n";
	while (!bh->Empty())
	{
		std::cout << bh->Top() << " ";
		bh->Pop();
	}
	std::cout << "\n";
	//Output:
	//	Building max heap over the 9 element array(43 1 6 3 41 51 36 23 11)
	//	Greatest value is 51
	//	Removing greatest value...
	//	Now greatest value is : 43
	//	Adding 50 to the heap...
	//	Rest of the values in the order as poped from the heap:
	//	50 43 41 36 23 11 6 3 1
	//	Now building a min heap over the same array.
	//	Lowest value is 1
	//	Removing lowest value...
	//	Lowest value is 3
	//	Removing lowest value...
	//	Lowest value is 6
	//	Removing lowest value...
	//	Change heap type to max heap again...
	//	Adding 25 to the heap...
	//	Rest of the values in the order as poped from the heap:
	//	51 43 41 36 25 23 11

	return 0;
}
*/
