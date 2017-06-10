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
void InsertionSort (T *array, int size, void *Compare)
{ 
    // If custiom comapre function is specified use static_cast it 
    // to bool function that takes two elements of type T.
    bool(*_compare)(T, T);
    bool use_custom_compare = (Compare != NULL);
    if (use_custom_compare)
        _compare = static_cast<bool(*)(T, T)>(Compare);
    
    // Iterate over whole array:
    for (int i = 1; i < size; ++i)
    {
        // Remember first element.
        T key = array[i];
        
        // Start from current index...
        int j = i;

        // And go towards 0...
        while (j > 0)
        {
            bool isGreater = (use_custom_compare) ? 
                utility::IsGreater(array[j-1], key, _compare) : 
                utility::IsGreater(array[j-1], key);

            // If given element is greater than key element marked up before entering loop...
            if (isGreater)
            {
                // Fix sort order by assining backwards untill it's ok.'
                array[j] = array[j-1];
                --j;
            }
            else
                break;
        }
        
        // Assing lost element which was stored in the key variable.
        array[j] = key;
    }
}

template <typename T>
void InsertionSort (T *array, int size)
{ 
    InsertionSort(array, size, NULL);
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
    InsertionSort (arr, 10);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr[i] << " ";
    InsertionSort (arr, 10, Greater);
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