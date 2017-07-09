/*===============================================================================
NOTE: Work in progress!
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Binary heap data structure. Used as a priority queue.
             Always gives the greatest element in the set, uses < operator only.
             Space complexity: O(n).
Operations:
	  * Push - insert new element. Time complexity: O(1).
	  * Pop - removes greatest element from the set. Time complexity: O(log(n)).
	  * Top - returns greatest element in the sett. Time complexity O(1).
	  * Size - returns the size of the heap (the number of elements in). Time complexity: O(1).
	  * Capacity - returns the capacity of the heap vector - does not have to be same as size. Time complexity: O(1).
	  * MaxSize - returns maximum capacity of the heap. Time complexity: O(1).
	  * Type - returns the type of the heap (maxHeap / minHeap). Time complexity: O(1).
	  * Empty - returns true if there is no element in heap. Time complexity: O(1).
	  * ChangeType - changes the type of the heap (form maxHeap to minHeap or vice versa) and rebuilds it. Time complexity: O(n*log(n)).
      * Union - merge two heaps into one. Time complexity: O(1).
Notes:
	  * 
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
class FibonacciHeap
{
    private:
        struct Node
        {
            T item;
            std::vector <int> childs;

            Node (T item) : item(item) {}
        };

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

        // All nodes in the heap.
        std::vector <Node *> nodes;

        // Simple GC. After removing item its index is stored here and can be reused 
        // again without further vector memory allocation.
        std::vector <int> freeIndexes; 

        // Indexes of nodes that don't have parent.
        std::vector <int> mainNodes;

        // Top value of the heap.
        T currentTopValue;

        // The size of the heap.
        int heapSize;

        // The type of the heap.
        HeapType heapType;

    public:
        FibonacciHeap() 
        {
            heapSize = 0;
        }

	    // Add an element to the set.
        void Add (T item)
        {
            Node *createdNode = new Node(item);
            int createdNodeIndex;

            // Use GC to save memory if possible or just add node to the nodes vector.
            if (freeIndexes.empty())
            {
                nodes.push_back(createdNode);
                createdNodeIndex = nodes.size()-1;
            }   
            else
            {
                createdNodeIndex = freeIndexes.back();
                nodes[createdNodeIndex] = createdNode;
                freeIndexes.pop_back();
            }

            // Put a node into main nodes panel, because it does not have a parent yet.
            mainNodes.push_back(createdNodeIndex);

            // Check if inserted value isn't better than current top value.
            if (HigherPriority(item, currentTopValue))
                currentTopValue = item;
            
            heapSize++;
        }

    	// Remove element with the highest priority from the set.
        void Pop()
        {
            //TODO:.
            return;
        }

        // Retuns element with the highest priority in the heap.
        void Top() { return currentTopValue; }


        // Returns the size of the heap, which is the number of elements currently in the container.
        std::size_t Size() 
        { 
            // TODO:.
            return 0;
        }

        // Returns the capacity of the heap.
        int Capacity() 
        { 
            // TODO:.
            return 0;
        } 

        // Returns the max size of the heap.
        long long MaxSize() 
        { 
            // TODO:.
            return 0;
        }
        
        // Returns the type of the heap.
        HeapType Type() 
        { 
            // TODO:.
            return 0; 
        }
        
        // Returns true if there is no emelent in the heap.
        bool Empty() 
        { 
            // TODO:.
            return false; 
        }

        // Change the type of the heap and rebuild it. This function is the slowest in the data strucutre, 
        // as its builds the whole heap again. Not recommended to use often.
        void ChangeType(HeapType newType)
        {
            // If requested type is an old one do nothing.
            if (newType == heapType)
                return;

            // Change the type of the heap.
            heapType = newType;
            
            // TODO:.
            return;
        }

        // Merge two heaps into one.
        void Union(FibonacciHeap<T> *other)
        {
            TODO:.
            return;
        }
};

int main ()
{
    FibonacciHeap<int> *fh = new FibonacciHeap<int>();

}