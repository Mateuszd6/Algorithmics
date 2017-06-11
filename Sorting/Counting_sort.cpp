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
void CountingSort(T *array, int size, int data_range, void *GetIndex)
{
    int(*_getIndex)(T) = static_cast<int(*)(T)>(GetIndex);

    // Array used to store result.
    T *result = new T[size];

    // Temporary array to store indexes.
    int *tmp = new int[data_range+1]();
    
    // TODO: replace array[i] with GetIndex(array[i])
    // After this loop tmp[i] will contain number of elements that GetIndex
    // will return i.
    for (int i = 0; i < size; ++i)
        tmp[_getIndex(array[i])]++;
    
    // After this loop tmp[i] will contain number of elements that GetIndex 
    // will return less or equal i.
    for (int i = 1; i <= data_range; ++i)
        tmp[i] += tmp[i-1];
    
    // Iterate backwards over the input array (backward iteration is used to keep sort stable).
    for (int i = size-1; i >= 0; --i)
    {
        // Use the index calcuated to indicate the new index of the element.
        result[tmp[_getIndex(array[i])]-1] = array[i];

        // Decrement the index by one.
        tmp[_getIndex(array[i])]--;
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
        arr1[i] = rand()%5;
    for (int i = 0; i < 10; ++i)
        std::cout << arr1[i] << " ";
    CountingSort (arr1, 10, 5, (void *)GetIndexInt);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr1[i] << " ";
    std::pair<int,int> *arr2 = new std::pair<int,int>[10];
    std::cout << "\nInput pair array: ";
    for (int i = 0; i < 10; ++i)
        arr2[i] = std::make_pair(rand()%5, rand()%5);
    for (int i = 0; i < 10; ++i)
        std::cout << "(" << arr2[i].first << ";" << arr2[i].second << ") ";
    CountingSort (arr2, 10, 5, (void *)GetIndexPair);
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