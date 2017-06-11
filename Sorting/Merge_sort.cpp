/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Sorts the given array. Time complexity: O(n*log(n)). Space complexity: O(n).
Notes:
       * Lower then Quicksort and doesn't' sort in place.
       * Temporary memory arrays are allocated once ot he beggining of the algorithm.
===============================================================================*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "..\Utility.cpp"

template <typename T>
void Merge (T* array, int begin1, int end1, int begin2, int end2, 
            bool (*Compare)(T, T), T* tmp_left, T *tmp_right)
{
    // If custiom comapre function is specified use static_cast it 
    // to bool function that takes two elements of type T.
    bool use_custom_compare = (Compare != NULL);

    // This are indexes of next not-assigned element in both temporary arrays.
    int next_in_left = 0, next_in_right = 0;
    
    // Compy intervals to temporary arrays.
    memcpy(tmp_left, array + begin1, sizeof(T)*(end1-begin1+1));
    memcpy(tmp_right, array + begin2, sizeof(T)*(end2-begin2+1));

    // Because both intervals are sorted always take the first one in the array,
    // Either left or right and assing it to the main array.
    for (int i = begin1; i <= end2; ++i)
    {
        bool isLower = (use_custom_compare) ? 
            utility::IsLower(tmp_left[next_in_left], tmp_right[next_in_right], Compare) :
            utility::IsLower(tmp_left[next_in_left], tmp_right[next_in_right]);

        // Special cases when end of one tmp arrays is reached:
        if (next_in_right == end2-begin2+1)
            array[i] = tmp_left[next_in_left++];
        else if (next_in_left == end1-begin1+1)
            array[i] = tmp_right[next_in_right++];

        // Normal cases:
        else if (isLower)
            array[i] = tmp_left[next_in_left++];
        else
            array[i] = tmp_right[next_in_right++];
    }
}

// Recursive call for MergeSort algorithm.
template <typename T>
void _mergeSort (T *array, int begin, int end, bool (*Compare)(T, T), T *tmp_left, T *tmp_right)
{
    // If the interval is not valid simply return.
    if (begin >= end)
        return;
    
    // Callculate the middle of the interval and call function recursivly.
    int m = (end-begin+1)/2;
    _mergeSort (array, begin, begin+m-1, Compare, tmp_left, tmp_right);
    _mergeSort (array, begin+m, end, Compare, tmp_left, tmp_right);
    
    // Merge two sorted interval into one.
    Merge (array, begin, begin+m-1, begin+m, end, Compare, tmp_left, tmp_right);
}

// Sort the given array. Use custom compare function.
template <typename T>
void MergeSort(T * array, int size, bool (*Compare)(T, T))
{
    if (size > 1)
    {
        T *tmp_left = new T[(size + 3)/2];
        T *tmp_right = new T[(size + 3)/2];
        _mergeSort(array, 0, size-1, Compare, tmp_left, tmp_right);
        
        delete[] tmp_left;
        delete[] tmp_right;
    }
}

// Sort the given array. Use '<' operator.
template <typename T>
void MergeSort(T * array, int size)
{
    if (size > 1)
    {
        T *tmp_left = new T[(size + 3)/2];
        T *tmp_right = new T[(size + 3)/2];
        _mergeSort(array, 0, size-1, (bool(*)(T,T))NULL, tmp_left, tmp_right);

        delete[] tmp_left;
        delete[] tmp_right;
    }
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
    MergeSort (arr, 10);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr[i] << " ";
    MergeSort (arr, 10, Greater);
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