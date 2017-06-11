/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Sorts the given array. Time complexity: O(n^2). Space complexity: O(1).
Notes:
       * Slow and not recommened for large arrays.
       * Sorts in place. No other data strucutres are needed for this algorithm to work.
===============================================================================*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "..\Utility.cpp"

template <typename T>
void SelectionSort (T *array, int size, bool(*Compare)(T,T))
{
    // If custiom comapre function is specified use static_cast it 
    // to bool function that takes two elements of type T.
    bool use_custom_compare = (Compare != NULL);
    
    // The index of the lower element.
    int min_index;

    // Iterate over the array...
    for (int i = 0; i < size; ++i)
    {
        // min_index is an index of lowest element in the interval.
        min_index = i;

        // For every element in the array...
        for (int j = i+1; j < size; ++j)
        {
            bool isLower = (use_custom_compare) ? 
                utility::IsLower(array[j], array[min_index], Compare) : 
                utility::IsLower(array[j], array[min_index]);
            // If it is lower than current element with a min_index change the index.
            if (isLower)
                min_index = j;
        }

        // Swap i'th element with the lowest found.
        std::swap (array[i], array[min_index]);
    }
}

template <typename T>
void SelectionSort (T *array, int size)
{
    SelectionSort(array, size, (bool(*)(T,T))NULL);
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
    SelectionSort (arr, 10);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr[i] << " ";
    SelectionSort (arr, 10, Greater);
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