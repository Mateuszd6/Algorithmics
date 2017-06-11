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
#include <ctime>

#define COUNTING_SORT_GET_INDEX(VAL) ((use_get_index_with_info) ? GetIndexWithInfo(VAL, AditionalGetIndexInfo) : GetIndex(VAL)) 

template <typename T>
void _countingSort(T *array, int size, int(*GetIndex)(T), int(*GetIndexWithInfo)(T, void*), 
    void *AditionalGetIndexInfo)
{
    bool use_get_index_with_info = (GetIndexWithInfo != NULL);

    // Search the array and find largest and lowest indexes.
    int max_index = COUNTING_SORT_GET_INDEX(array[0]);
    int min_index = COUNTING_SORT_GET_INDEX(array[0]);
    for (int i = 0; i < size; ++i)
    {
        int index = COUNTING_SORT_GET_INDEX(array[i]);
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
        tmp[COUNTING_SORT_GET_INDEX(array[i])-min_index]++;
    
    // After this loop tmp[i] will contain number of elements that GetIndex 
    // will return less or equal i - min_index.
    for (int i = 1; i <= data_range; ++i)
        tmp[i] += tmp[i-1];
    
    // Iterate backwards over the input array (backward iteration is used to keep sort stable).
    for (int i = size-1; i >= 0; --i)
    {
        // Use the index calcuated to indicate the new index of the element.
        result[tmp[COUNTING_SORT_GET_INDEX(array[i])-min_index]-1] = array[i];

        // Decrement the index by one.
        tmp[COUNTING_SORT_GET_INDEX(array[i])-min_index]--;
    }
    
    // Copy array used to store result to the input array.
    memcpy(array, result, size*sizeof(T));

    // Delete allocated arrays.
    delete[] result;
    delete[] tmp;
}

template <typename T>
void CountingSort(T *array, int size, int(*GetIndex)(T))
{
    _countingSort(array, size, GetIndex, (int(*)(T, void*))NULL, NULL);
}

template <typename T>
void CountingSort(T *array, int size, int(*GetIndexWithInfo)(T, void*), void *AditionalInfo)
{
    _countingSort(array, size, (int(*)(T))NULL, GetIndexWithInfo, AditionalInfo);
}

/*
//============== USAGE EXAMPLE ===================

// Function used by counting sort when sorting inteegers.
int GetIndexInt (int self)
{
    return self;
}

// Function used by counting sort when sorting inteeger pairs.
int GetIndexPair (int *self, void *pindex)
{
    int *idx = (int *)pindex;
    return self[(* idx)];
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
    CountingSort (arr1, 10, GetIndexInt);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << arr1[i] << " ";
    int **arr2 = new int* [10];
    std::cout << "\nInput pair array: ";
    for (int i = 0; i < 10; ++i)
    {
        arr2[i] = new int[2];
        arr2[i][0] = rand()%5;
        arr2[i][1] = rand()%5;
    }
    for (int i = 0; i < 10; ++i)
        std::cout << "(" << arr2[i][0] << ";" << arr2[i][1] << ") ";
    int sort_by_index = 0;
    CountingSort (arr2, 10, GetIndexPair, &sort_by_index);
    std::cout << "\nSorted array (by first): ";
    for (int i = 0; i < 10; ++i)
        std::cout << "(" << arr2[i][0] << ";" << arr2[i][1] << ") ";
    //Possible output:
    //  Input int array: 5 5 7 4 6 3 3 3 4 7
    //  Sorted array: 3 3 3 4 4 5 5 6 7 7
    //  Input pair array: (3;3) (2;4) (3;4) (1;4) (0;0) (3;1) (2;2) (3;1) (0;3) (0;2)
    //  Sorted array (by first): (0;0) (0;3) (0;2) (1;4) (2;4) (2;2) (3;3) (3;4) (3;1) (3;1)
    
    return 0;
}
*/