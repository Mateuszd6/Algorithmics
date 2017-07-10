/*===============================================================================
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
	  * ChangeType - changes the type of the heap (form maxHeap to minHeap or vice versa) and rebuilds it. Time complexity: O(n).
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
            T value;
            std::vector <int> childs;

            Node (T value) : value(value) {}

            int Degree() { return childs.size(); }
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
        int currentTopNodeIndex;

        // The size of the heap.
        int heapSize;

        // Max degree of the node in the heap which is log_2(heapSize).
        int maxDegree;

        // Set the heap size and update maxDegree value.
        void SetHeapSize(int newSize)
        {
            heapSize = newSize;

            // Oldest bit is equal to log_2(newSize)+1.
            maxDegree = utility::GetHighestBit(newSize) + 2;
        }

        // The type of the heap.
        HeapType heapType;

        // Called after Pop operation organizes root nodes and their heaps in sucha a way,
        // there are no roots with the same degree.
        void FixHeap()
        {   
            // Array contains root indexes. A[i] constains root with degree equal to i.
            // When value is equal to -1 it means there is no such root found yet.
            int *A = new int[maxDegree];

            for (int i = 0; i < maxDegree; ++i)
                A[i] = -1;

            // Iterate over root nodes...
            for (int i = 0; i < mainNodes.size(); ++i)
            {
                int currentNodeIndex = i;

                // Search for other node with same degree and merge with current if found.
                while (A[nodes[mainNodes[currentNodeIndex]]->Degree()] != -1)
                {
                    int otherNodeIndex = A[nodes[mainNodes[currentNodeIndex]]->Degree()];
                    if (HigherPriority(nodes[mainNodes[otherNodeIndex]]->value, nodes[mainNodes[currentNodeIndex]]->value))
                        std::swap(mainNodes[currentNodeIndex], mainNodes[otherNodeIndex]);

                    // assert(otherNodeIndex < currentNodeIndex);

                    A[nodes[mainNodes[currentNodeIndex]]->Degree()] = -1;
                    nodes[mainNodes[currentNodeIndex]]->childs.push_back(mainNodes[otherNodeIndex]);
                }
                A[nodes[mainNodes[currentNodeIndex]]->Degree()] = currentNodeIndex;
            }

            // Create new mainNodes array based on a previous one and A array.
            bool topValueWasSet = false;
            for (int i = 0; i < maxDegree; ++i)
            {
                if (A[i] != -1)
                    A[i] = mainNodes[A[i]];
            }

            mainNodes.clear();
            for (int i = 0; i < maxDegree; ++i)
            {
                if (A[i] != -1)
                {
                    if (!topValueWasSet)
                    {
                        currentTopNodeIndex = mainNodes.size();
                        topValueWasSet = true;
                    }
                    else if (HigherPriority(nodes[A[i]]->value, nodes[mainNodes[currentTopNodeIndex]]->value))
                        currentTopNodeIndex = mainNodes.size();
               
                    mainNodes.push_back(A[i]);
                }
            }

            delete[] A;

        }

    public:
        FibonacciHeap(HeapType heapType) : heapType(heapType) 
        {
            SetHeapSize(0);
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
                freeIndexes.pop_back();
                nodes[createdNodeIndex] = createdNode;
            }

            // Put a node into main nodes panel, because it does not have a parent yet.
            mainNodes.push_back(createdNodeIndex);

            // If pushed element is one and only item in the heap.
            if (mainNodes.size() == 1)
                currentTopNodeIndex = 0;

            // Check if inserted value isn't better than current top value.
            else if (HigherPriority(item, nodes[mainNodes[currentTopNodeIndex]]->value))
                currentTopNodeIndex = mainNodes.size()-1;
            
            SetHeapSize(heapSize + 1);
        }

    	// Remove element with the highest priority from the set.
        void Pop()
        {
            if (Empty())
                return;
                
            Node *topNode = nodes[mainNodes[currentTopNodeIndex]];
            freeIndexes.push_back(mainNodes[currentTopNodeIndex]);
            
            std::swap(mainNodes[currentTopNodeIndex], mainNodes.back());
            mainNodes.pop_back();


            for (auto i : topNode->childs)
                mainNodes.push_back(i);

            FixHeap();

            SetHeapSize(heapSize - 1);
        }

        // Retuns element with the highest priority in the heap.
        T Top() { return nodes[mainNodes[currentTopNodeIndex]]->value; }


        // Returns the size of the heap, which is the number of elements currently in the container.
        std::size_t Size() 
        { 
            return (std::size_t)heapSize;
        }

        // Returns the capacity of the heap.
        int Capacity() 
        { 
            return nodes.capacity;
        } 

        // Returns the max size of the heap.
        long long MaxSize() 
        { 
            return nodes.max_size;
        }
        
        // Returns the type of the heap.
        HeapType Type() 
        { 
            return heapType; 
        }
        
        // Returns true if there is no emelent in the heap.
        bool Empty() 
        { 
            return (Size() == 0);
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

            // Mark which node is obsolate, because of being removed from the heap earlier.            
            bool *was_deleted = new bool[nodes.size()+1]();
            for (int i = 0; i < freeIndexes.size(); ++i)
                was_deleted[freeIndexes[i]] = true;

            // Put every not-obsolate node in the main nodes list and clear their childs.
            mainNodes.clear();
            bool topValueWasSet = false;
            for (int i = 0; i < nodes.size(); ++i)
            {
                if (!was_deleted[i])
                {
                    mainNodes.push_back(i);
                    if (!topValueWasSet || HigherPriority(
                        nodes[i]->value, nodes[mainNodes[currentTopNodeIndex]]->value))
                        {
                            currentTopNodeIndex = mainNodes.size()-1;
                            topValueWasSet = true;
                        }
                    nodes[i]->childs.clear();

                }
            }

            delete[] was_deleted;

            // NOTE: There is no need to call FixHeap yet. It will be called when Pop is called.
        }
};

/*
//============== USAGE EXAMPLE ===================

int main()
{
    FibonacciHeap<int> *fh = new FibonacciHeap<int>(maxHeap);
	std::cout << "Adding 12 to the heap...\n";
    fh->Add(12);
	std::cout << "Adding 5 to the heap...\n";
    fh->Add(5);
	std::cout << "Adding 4 to the heap...\n";
    fh->Add(4);
	std::cout << "Adding 2 to the heap...\n";
    fh->Add(2);
	std::cout << "Adding 8 to the heap...\n";
    fh->Add(8);
	std::cout << "Now greatest value is: " << fh->Top() << "\n";
    std::cout << "Removing greatest value...\n";
    fh->Pop();
    fh->ChangeType(minHeap);
    std::cout << "Changing heap type. Now highest priority has the lowest element.\n";
    for (int i = 0; i < 2; ++i)
    {
	    std::cout << "Now lowest value is: " << fh->Top() << "\n";
        std::cout << "Removing lowest value...\n";
        fh->Pop();
    }
    fh->ChangeType(maxHeap);
    std::cout << "Changing heap type. Now highest priority has the greatest element.\n";
	std::cout << "Adding 15 to the heap...\n";
    fh->Add(15);
	std::cout << "Adding 20 to the heap...\n";
    fh->Add(20);
    while (!fh->Empty())
    {
        std::cout << "Now greatest value is: " << fh->Top() << "\n";
        std::cout << "Removing greatest value...\n";
        fh->Pop();
    }
    fh->Add(9);
	std::cout << "Adding 9 to the heap...\n";
    fh->Add(11);
	std::cout << "Adding 11 to the heap...\n";
    while (!fh->Empty())
    {
        std::cout << "Now greatest value is: " << fh->Top() << "\n";
        std::cout << "Removing greatest value...\n";
        fh->Pop();
    }

    //Output:
    //  Adding 12 to the heap...
    //  Adding 5 to the heap...
    //  Adding 4 to the heap...
    //  Adding 2 to the heap...
    //  Adding 8 to the heap...
    //  Now greatest value is: 12
    //  Removing greatest value...
    //  Changing heap type. Now highest priority has the lowest element.
    //  Now lowest value is: 2
    //  Removing lowest value...
    //  Now lowest value is: 4
    //  Removing lowest value...
    //  Changing heap type. Now highest priority has the greatest element.
    //  Adding 15 to the heap...
    //  Adding 20 to the heap...
    //  Now greatest value is: 20
    //  Removing greatest value...
    //  Now greatest value is: 15
    //  Removing greatest value...
    //  Now greatest value is: 8
    //  Removing greatest value...
    //  Now greatest value is: 5
    //  Removing greatest value...
    //  Adding 9 to the heap...
    //  Adding 11 to the heap...
    //  Now greatest value is: 11
    //  Removing greatest value...
    //  Now greatest value is: 9
    //  Removing greatest value...
}
*/