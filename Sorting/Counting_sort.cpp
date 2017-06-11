/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Sorts the given array. Time complexity: O(n+k). Space complexity: O(n+k).
Notes:
       * This sort has linear time complexity if data range is O(n).
       * Every element must get its intex within [0; data_range] which is computed by GetIndex function.
       * This sort is stable (see usage example for details).
===============================================================================*/

#include <iostream>
#include <cstdlib>
#include <functional>
#include <ctime>

template <typename T>
void CountingSort(T *array, int size, void *GetIndex)
{
    // Function used to get index.
    int(*_getIndex)(T) = static_cast<int(*)(T)>(GetIndex);

    // Search the array and find largest and lowest indexes.
    int max_index = _getIndex(array[0]);
    int min_index = _getIndex(array[0]);
    for (int i = 0; i < size; ++i)
    {
        int index = _getIndex(array[i]);
        if (max_index < index)
            max_index = index;
        if (min_index > index)
            min_index = index;
    }

    // Data range is a difference between greates and lowest indexes.
    int data_range = max_index - min_index + 1;

    // Array used to store result.
    T *result = new T[size];

    // Temporary array to store indexes.
    int *tmp = new int[data_range+1]();
    
    // After this loop tmp[i] will contain number of elements that GetIndex
    // will return i - min_index;
    for (int i = 0; i < size; ++i)
        tmp[_getIndex(array[i])-min_index]++;
    
    // After this loop tmp[i] will contain number of elements that GetIndex 
    // will return less or equal i - min_index.
    for (int i = 1; i <= data_range; ++i)
        tmp[i] += tmp[i-1];
    
    // Iterate backwards over the input array (backward iteration is used to keep sort stable).
    for (int i = size-1; i >= 0; --i)
    {
        // Use the index calcuated to indicate the new index of the element.
        result[tmp[_getIndex(array[i])-min_index]-1] = array[i];

        // Decrement the index by one.
        tmp[_getIndex(array[i])-min_index]--;
    }
    
    // Copy array used to store result to the input array.
    memcpy(array, result, size*sizeof(T));

    // Delete allocated arrays.
    delete[] result;
    delete[] tmp;
}

/*
//============== USAGE EXAMPLE ===================

// Function used by counting sort when sorting inteegers.
int GetIndexInt (int self)
{
    return self;
}

// Function used by counting sort when sorting inteeger pairs.
int GetIndexPair (std::pair<int, int> self)
{
    return self.first;
}

int main()
{
	srand(time(NULL));
    int *arr1 = new int[10];
    std::cout << "Input int array: ";
    for (int i = 0; i < 10; ++i)
        arr1[i] = rand()%5+3;
    for (int i = 0; i < 10; ++i)
        std::cout << arr1[i] << " ";
    CountingSort (arr1, 10, (void *)GetIndexInt);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr1[i] << " ";
    std::pair<int,int> *arr2 = new std::pair<int,int>[10];
    std::cout << "\nInput pair array: ";
    for (int i = 0; i < 10; ++i)
        arr2[i] = std::make_pair(rand()%5, rand()%5);
    for (int i = 0; i < 10; ++i)
        std::cout << "(" << arr2[i].first << ";" << arr2[i].second << ") ";
    CountingSort (arr2, 10, (void *)GetIndexPair);
    std::cout << "\nSorted array (by first): ";
    for (int i = 0; i < 10; ++i)
        std::cout << "(" << arr2[i].first << ";" << arr2[i].second << ") ";
    //Possible output:
    //  Input int array: 3 1 2 3 2 3 0 4 0 0
    //  Sorted array: 0 0 0 1 2 2 3 3 3 4
    //  Input pair array: (3;3) (1;0) (2;2) (2;2) (2;1) (0;3) (1;4) (2;0) (2;0) (4;0)
    //  Sorted array (by first): (0;3) (1;0) (1;4) (2;2) (2;2) (2;1) (2;0) (2;0) (3;3) (4;0)

    return 0;
}
*/