/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Static interval tree created on top of the array. 
             It is a full binary tree implemented on the array. 
             It allows for interval queries as well as updating given range by given value. 
             It used two function pointers, first is used to set value of the current subtree
             based on child subtrees, value inserted into current node and the range of it
             (which is a number of leafs in the subtree).
             Second pointer points to the function used with update operation to update the value.
             Space complexity: O(n). 
Operations:
      * Update - Update every node in a given range by the given value. Time complexity: O(log(n)).
      * Query - Query given interval. Time complexity: O(log(n)).

Notes:
      * Inicialization (done in a constructor) takes O(n) time, where n is a size of given array.
      * Namespace ITreeUtility contains some presets of the interval trees that are commonly used.
===============================================================================*/

#include <iostream>
#include <vector>

template <class T>
class IntervalTree
{
private:
    // Node used by the tree.
    struct node
    {
        // This is value inserted to the current vertex;
        T inserted_value;

        // This is value containing all values in the current vertex subtree + inserted_value;
        // For leafs inserted_value == subtree_value.
        T subtree_value;

        // The range of the node. (For leafs left == right).
        int left, right;

        node () {}
    };
    
    // An array of nodes, used as a tree.
    node *tree;

    // A value return if an interval does not overlapped with queried one. 
    // For example: when asking for sum of the interval, neutral value should be 0,
    // when asking for max value, it should be INT_MIN.
    T neutral;

    // Given inserted_value, subtree_values of left and right childs and range of the interval, compute subtree_value for the node.
    T (*GetValue)(T, T, T, int);

    // Given inserted_value, subtree_values of left and right childs
    T (*SetValue)(T, T);

    // The size of the tree array 
    // It is usualy more than twice bigger than size specified in the constructor.
    int size;

    // Get the parent of the node with given ID.
    int Parent(int v) { return (v/2); }

    // Get the left child of the node with given ID.
    int LeftChild(int v) { return (2*v); }

    // Get the right child of the node with given ID.
    int RightChild(int v) { return (2*v + 1); }
    
    // True if node with given ID is a leaf.
    bool IsLeaf(int v) { return (v >= size); }

    // Recursive function to call query in a given node.
    // NOTE: subtree_range is number of nodes in the queried interval in the subtree of current node.
    T _Query (int current, int left, int right, int &subtree_range)
    {
        // Temporary variable to store the range.
        int range = 0;

        // If there is no overlap between the queried interval [left, right] and current interval...
        if ((tree[current].right < left) || (tree[current].left > right))
        {
            // retrurn the neutral value, so it won't spoil the result.
            return neutral;
        }

        // If current interval is fully inside the queried one...
        if (tree[current].left >= left && tree[current].right <= right)
        {
            // Range of interval is number of nodes is the subtree.
            subtree_range += tree[current].right - tree[current].left + 1;

            // Return computed value of the whole subtree.
            return tree[current].subtree_value;
        }

        // Queried intervals partially overlap each other.
        // Call this function on both childs.
        // NOTE: This part of code isn't reachable for leaf nodes, so out-of-range is impossible here.
        int res =  GetValue (tree[current].inserted_value,
             _Query(LeftChild(current), left, right, range), 
            _Query(RightChild(current), left, right, range),
            range);

        // Set the computed range and the result.
        subtree_range += range;
        return res;
    }
    
    // Recursive function to update recursivly in a given node.
    void _Update (int current, int left, int right, T value)
    {
        // If there is no overlap between the queried interval [left, right] and current interval...
        if ((tree[current].right < left) || (tree[current].left > right))
            // Return as it is nothing to do.
            return;

        // If current interval is fully inside the [left, right] one...
        if (tree[current].left >= left && tree[current].right <= right)
        {
            // Set the value using SetValue function specyfied in the constructor.
            tree[current].inserted_value = SetValue(tree[current].inserted_value, value);
            
            // If current vertex is a leaf, then subtree_value == inserted_value.
            if (IsLeaf(current))
                tree[current].subtree_value = tree[current].inserted_value;
            
            // Otherwise use GetValue to compute it.
            else
                tree[current].subtree_value = GetValue(tree[current].inserted_value, 
                    tree[LeftChild(current)].subtree_value, 
                    tree[RightChild(current)].subtree_value, 
                    tree[current].right - tree[current].left + 1);

            return;
        }

        // If interval are partailly overlapped call this method on both childs.
        // NOTE: This part of code isn't reachable for leaf nodes, so out-of-range is impossible here.
        _Update(LeftChild(current), left, right, value);
        _Update(RightChild(current), left, right, value);
        
        // And use GetValue function to compute subtree_value.
        tree[current].subtree_value = GetValue(tree[current].inserted_value, 
            tree[LeftChild(current)].subtree_value, 
            tree[RightChild(current)].subtree_value, 
            tree[current].right - tree[current].left + 1);
    }

public:
    // Update the given interval with given value.
    void Update (int left, int right, T value)
    {
        _Update(1, left, right, value);
    }

    // Get the query from given interval.
    T Query (int left, int right)
    {
        int __tmp = 0;
        return _Query(1, left, right, __tmp);
    }
    
    // total_size is a size of the array, and *array is a pinter to it.
    // It is an array on top which interval tree will be created.
    // func_get_value is a pointer to template function T (T,T,T,int) which gets:
    // inserted_value to the node, value of the left subtree, value of the right subtree, 
    // and the range of interval and returs the computed value (which is a value of current subtree).
    // func_set_value is a pointer to template function T(T,T) which gets:
    // inserted_value to the node and an update value and returns new computed value.
    // neutral_value is a value that is returned if an interval does not overlapped with queried one. 
    // For example: when asking for sum of the interval, neutral value should be 0,
    // when asking for max value, it should be INT_MIN.
    IntervalTree (int total_size, T *array, 
        void *func_get_value, void *func_set_value, T neutral_value)
    {
        // Compute the size of the tree array which will be smallest power of 2 not less that total_size * 2. 
        size = 1;
        while (size < total_size)
            size *= 2;

        // Allocate the nodes array.
        tree = new node[2*size];

        // Assing the function pointers.
        GetValue = (T (*)(T, T, T, int))func_get_value;
        SetValue = (T (*)(T, T))func_set_value;
        
        // Assing the neutral value.
        neutral = neutral_value;

        // Assing the leaf nodes based on the values from the array.
        for (int i = 0; i < total_size; ++i)
        {
            tree[size+i].left = i;
            tree[size+i].right = i;
            tree[size+i].inserted_value = array[i];
            tree[size+i].subtree_value = array[i];
        }

        // Assing the rest of the leaf nodes that are out of range, but were allocated since
        // whole power of 2 is needed for this structure to work well.
        for (int i = total_size; i < size; ++i)
        {
            tree[size+i].left = i;
            tree[size+i].right = i;
            tree[size+i].inserted_value = neutral;
            tree[size+i].subtree_value = neutral;
        }

        // Compute the values of not-leaf nodes.
        for (int i = size-1; i >= 1; --i)
        {
            tree[i].left = tree[LeftChild(i)].left;
            tree[i].right = tree[RightChild(i)].right;
            tree[i].inserted_value = neutral;
            tree[i].subtree_value = GetValue(tree[i].inserted_value, tree[LeftChild(i)].subtree_value, tree[RightChild(i)].subtree_value, tree[i].right - tree[i].left + 1);
        }
    }
    
    // Destructor.
    ~IntervalTree()
    {
        delete[] tree;
    }
};

// The namespace containing some template, commonly used interval tree classes
// and function that they need to work.
namespace ITreeUtility
{
    inline int GetSum (int inserted_value, int left_subtree, int right_subtree, int range)
    {
        int res = ((inserted_value * range) + left_subtree + right_subtree);
        return res;
    }

    inline int GetMax (int inserted_value, int left_subtree, int right_subtree, int range)
    {
        return (left_subtree > right_subtree) ? 
            (left_subtree + inserted_value) : 
            (right_subtree + inserted_value);
    }

    inline int GetMin (int inserted_value, int left_subtree, int right_subtree, int range)
    {
        return (left_subtree < right_subtree) ? 
            (left_subtree + inserted_value) : 
            (right_subtree + inserted_value);
    }

    inline int SetSum (int inserted_value, int new_value)
    {
        return inserted_value + new_value;
    }

    inline int SetMax (int inserted_value, int new_value)
    {
        return (inserted_value > new_value) ? inserted_value : new_value;
    }

    inline int SetMin (int inserted_value, int new_value)
    {
        return (inserted_value < new_value) ? inserted_value : new_value;
    }

    // Increment the values on the given range.
    // Get the sum of the given range.
    class IncrementSumIntervalTree : public IntervalTree<int>
    {
    public:
        IncrementSumIntervalTree (int total_size, int *array) : 
            IntervalTree(total_size, array, GetSum, SetSum, 0) {}
    };

    // Increment the values on the given range.
    // Get the biggest value in the given range.
    class IncrementMaxIntervalTree : public IntervalTree<int>
    {
    public:
        IncrementMaxIntervalTree (int total_size, int *array) : 
            IntervalTree(total_size, array, GetMax, SetSum, 0) {}
    };

    // Increment the values on the given range.
    // Get the lowest value in the given range.
    class IncrementMinIntervalTree : public IntervalTree<int>
    {
    public:
        IncrementMinIntervalTree (int total_size, int *array) : 
            IntervalTree(total_size, array, GetMin, SetSum, 0) {}
    };
};

/*
//============== USAGE EXAMPLE ===================

int main ()
{
    int * arr = new int[7];
    std::cout << "Making intetval tree based on array: [ ";
    for (int i = 0; i < 7; ++i) 
    {
        arr[i] = i;
        std::cout << arr[i] << " ";
    }
    std::cout << "]\n";
    IntervalTree<int> itree(7, arr, ITreeUtility::GetSum, ITreeUtility::SetSum, 0);
    std::cout << "Incrementing range: [0-3] by +3\n";
    itree.Update(0, 3, 3);
    std::cout << "Sum of range [1-2] is " << itree.Query(1, 2) << "\n";
    //Output:
    //  Making intetval tree based on array: [ 0 1 2 3 4 5 6 ]
    //  Incrementing range: [0-3] by +3
    //  Sum of range [1-2] is 9
    
    delete[] arr;
    
    arr = new int[9];
    std::cout << "Making intetval tree based on array: [ ";
    for (int i = 0; i < 9 ; ++i)
    {
        std::cout << "0 ";
        arr[i] = 0;
    }
    std::cout << "]\n";
    ITreeUtility::IncrementMaxIntervalTree my_tree(9, arr);
    my_tree.Update(1, 3, 5);
    std::cout << "Incrementing range: [1-3] by +5\n";
    my_tree.Update(0, 5, 1);
    std::cout << "Incrementing range: [0-5] by +1\n";
    std::cout << "Largest value at range [2-4] is " << my_tree.Query(2,4) << "\n";
    //Output:
    //  Making intetval tree based on array: [ 0 0 0 0 0 0 0 0 0  ]
    //  Incrementing range: [1-3] by +5
    //  Incrementing range: [0-5] by +1
    //  Largest value at range [2-4] is 6
    
    delete[] arr;

    return 0;
}
*/