/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Simple strucutre for placing items on the end, poping them from the front 
             and getting the maximum value. Every operation is performed in amortiszed constant time.
Operations:
      * PushBack - Insert an element to the end of the queue. Time complexity: O(1). (Amortized, O(n) in worst case).
      * PopFront - Pop one element from the front of the queue. Time complexity: O(1).
      * GetMax - Get element with the biggest value. Time complexity: O(1). 
      * Size - returns the size of the queue (the number of elements in). Time complexity: O(1).
	  * Empty - returns true if there is no element in queue. Time complexity: O(1).

Notes:
      * Only items that have potencial to be the maximum value are stored in the queue.
      * The rest isn't stored, but every 'ímportant' item stores info how many unimportant ones are between it and next important one. 
===============================================================================*/

#include <iostream>
#include <deque>
#include "..\Utility.cpp"

template <class T>
class KMaxQueue
{
private:
    struct Node
    {
        // Number of 'uninteresting' items between item stored in the node and 
        // next item with the value greater or equal to this one. 
        int unimportantBefore;

        // Item stored in the node.
        T value;

        Node (T value, int unimportantBefore) : value(value), unimportantBefore(unimportantBefore) { }
    };

    std::deque<Node *> *nodes;

    std::size_t size;

public:
    KMaxQueue()
    {
        nodes = new std::deque<Node *>();
        size = 0;
    } 

    ~KMaxQueue()
    {
        while (!nodes->empty())
        {
            delete nodes->front();
            nodes->pop_front();
        }
        delete nodes;
    }

    // Push new element to the end of the queue.
    void PushBack(T item) 
    {
        int unimportantBefore = 0;
        while (!nodes->empty() && utility::IsGreater(item, nodes->back()->value))
        {
            // Previous node 'interesting' item now become unimportant. 
            // This item and all unimportant items before now are stored as 
            // unimportant values of inserting item.
            unimportantBefore += nodes->back()->unimportantBefore + 1;
            nodes->pop_back();
        }
        nodes->push_back(new Node(item, unimportantBefore));
        size++;
    }

    // Pop the first element from the front.
    void PopFront()
    {
        if (Empty())
        {
            std::cerr << "Queue is already empty.\n";
            return;
        }
        
        // If there is any unimportant item before item stored in the first node in the queue,
        // decrease number of unimportant items before it.
        if (nodes->front()->unimportantBefore > 0)
            nodes->front()->unimportantBefore--;
        
        // Remove important value by poping the front node.
        else
            nodes->pop_front();
        
        size--;
    }

    // Get the value of biggest element in the queue.
    T GetMax()
    {
        if (Empty())
            throw ("Queue is empty.");
        return nodes->front()->value;
    }

	// Returns the size of the queue, which is the number of elements currently in the container.
    std::size_t Size() { return size; }

	// Returns true if there is no emelent in the queue.
    bool Empty() { return (size == 0); }
};

/*
//============== USAGE EXAMPLE ===================

int main()
{
    KMaxQueue<int> *queue = new KMaxQueue<int>();
    std::cout << "Adding " << 4 << " to the queue...\n"; 
    queue->PushBack(4);
    std::cout << "Adding " << 5 << " to the queue...\n"; 
    queue->PushBack(5);
    std::cout << "Max value is: " << queue->GetMax() << "\n";
    std::cout << "Adding " << 3 << " to the queue...\n"; 
    queue->PushBack(3);
    std::cout << "Adding " << 1 << " to the queue...\n"; 
    queue->PushBack(1);
    std::cout << "Adding " << 1 << " to the queue...\n"; 
    queue->PushBack(1);
    std::cout << "Adding " << 1 << " to the queue...\n"; 
    queue->PushBack(1);
    std::cout << "Adding " << 2 << " to the queue...\n"; 
    queue->PushBack(2);
    std::cout << "Poping from the queue...\n";
    queue->PopFront();
    std::cout << "Max value is: " << queue->GetMax() << "\n";
    std::cout << "Poping from the queue...\n";
    queue->PopFront();
    std::cout << "Poping from the queue...\n";
    queue->PopFront();
    std::cout << "Max value is: " << queue->GetMax() << "\n";
    //Output:
    //  Adding 4 to the queue...
    //  Adding 5 to the queue...
    //  Max value is: 5
    //  Adding 3 to the queue...
    //  Adding 1 to the queue...
    //  Adding 1 to the queue...
    //  Adding 1 to the queue...
    //  Adding 2 to the queue...
    //  Poping from the queue...
    //  Max value is: 5
    //  Poping from the queue...
    //  Poping from the queue...
    //  Max value is: 2
    
    return 0;
}
*/