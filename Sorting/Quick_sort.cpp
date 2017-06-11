/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Sorts the given array. Time complexity: O(n*log(n)). Space complexity: O(1).
Notes:
       * It works in any case, it is not possible to slow this algorithm down with any data.
       * It's not using Hore's partition. RandomPartition function devides given array into separate pieces.
       * Firstly pivot is being picked. Array interval is devided for (possibly) 3 segments:
       * [Lower than pivot] [Equal to pivot] [Greater than pivot]
       * RandomPartition function returns a strucure that holds las index of 
         [Lower than pivot] group and first index of [Greater than pivot] group.
===============================================================================*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "..\Utility.cpp"

// Strucutre return by Partition function.
struct PartitionInfo
{
    int low, high;

    PartitionInfo () {}
    PartitionInfo (int low, int high) : low(low), high(high) {};
};

template <typename T>
PartitionInfo RandomPartition (T *array, int begin, int end, bool(*Compare)(T, T))
{
    // If custiom comapre function is specified use static_cast it 
    // to bool function that takes two elements of type T.
    bool use_custom_compare = (Compare != NULL);
    
    // The strucutre that holds the result.
    PartitionInfo res;

    // Randomise pivot choice and put it to the end of the interval.
    int pivot_index = begin + rand()%(end-begin+1); 
    std::swap(array[pivot_index], array[end]);
    
    // First greater than pivot element index.
    int high = -1;
    
    // Last lower than pivot element index.
    int  low = -1;

    // Pivot index. It may change druing the algorithm.
    int part = end;

    // Iterate over the interval
    for (int i = 0; i < part; ++i)
    {
        bool isLower = (use_custom_compare) ? 
            utility::IsLower(array[i], array[part], Compare) : 
            utility::IsLower(array[i], array[part]);
            
        bool isGreater = (use_custom_compare) ? 
            utility::IsGreater(array[i], array[part], Compare) : 
            utility::IsGreater(array[i], array[part]);

        // If element is less than pivot...
        if (isLower)
        {
            // If it there wasn't any elemet greater than pivot till now...
            if (high == -1)
                // Last low index simply increases.
                low++;
            // In another case...
            else
            {
                // Move both low and hight indexes and swap two elements on the ends of the interval.
                low++;
                high++;
                std::swap (array[low], array[high]);
            }
        }
        
        // If element is greater than pivot...
        else if (isGreater)
        {
            // If it there wasn't any elemet greater than pivot till now...
            if (high == -1)
                // This is the first greater element index.
                high = i;
            else
                // Otherwise just increment it.
                high++;
        }

        // If the element is equal to pivot.
        else
        {
            // Move it the the back by swapping with any element there and replace current step.
            --part;
            std::swap (array[i], array[part]);
            --i;
        }
    }

    // handle possible special-cases where no greater, no lower element or both were not found:
    // In this case every element in the interval is equal to pivot.
    if (low == -1 && high == -1)
    {
        res.low = begin;
        res.high = end;
    }

    // The [Less than pivot] segment is empty. 
    else if (low == -1)
    {
        for (int i = part; i <= end; ++i)
            std::swap(array[low+i-part+1], array[i]);
        res.low = begin;
        res.high = //max(low, 0)+1+end-part+1;
        (low >= 0) ? low+1+end-part+1 : 1+end-part+1;
    }

    // The [Greather than pivot] segment is empty.
    else if (high == -1)
    {
        res.low = low;
        res.high = end;
    }

    // Every segment has at least one emement in it.
    else
    {
        // Swap the pivot and all elements equal to it with elements greather than it. 
        for (int i = part; i <= end; ++i)
            std::swap(array[low+i-part+1], array[i]);

        res.low = low;
        res.high = //max (low, 0)+1+end-part+1;
        (low >= 0) ? low+1+end-part+1 : 1+end-part+1;
    }

    // Return the result.
    return res;
}

// Recursive call for Quicksort algorithm.
template <typename T>
void _quickSort (T *array, int begin, int end, bool(*Compare)(T,T))
{
    // If the interval is not valid simply return.
    if (begin >= end)
        return;
    
    // Otherwise split the interval into three parts and do recursive calls.
    PartitionInfo info = RandomPartition(array, begin, end, Compare);
    _quickSort (array, begin, info.low, Compare);
    _quickSort (array, info.high, end, Compare);
}

// Sort the given array. Use custom compare function.
template <typename T>
void QuickSort(T * array, int size, bool(*Compare)(T,T))
{
    if (size > 1)
        _quickSort(array, 0, size-1, Compare);
}

// Sort the given array. Use '<' operator.
template <typename T>
void QuickSort(T * array, int size)
{
    if (size > 1)
        _quickSort(array, 0, size-1, (bool(*)(T,T))NULL);
}

/*
//============== USAGE EXAMPLE ===================

// Function used for different than default comparation. 
bool Greater (int self, int other)
{
    return self > other;
}

int main()
{
	srand(time(NULL));
    int *arr = new int[10];
    std::cout << "Input array: ";
    for (int i = 0; i < 10; ++i)
        arr[i] = rand()%100;
    for (int i = 0; i < 10; ++i)
        std::cout << arr[i] << " ";
    QuickSort (arr, 10);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr[i] << " ";
    QuickSort (arr, 10, Greater);
    std::cout << "\nSorted array by using custom function: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr[i] << " ";
    //Possible output:
    //  Input array: 74 59 14 95 64 59 44 2 78 61
    //  Sorted array: 2 14 44 59 59 61 64 74 78 95
    //  Sorted array by using custom function: 95 78 74 64 61 59 59 44 14 2

    return 0;
}
*/