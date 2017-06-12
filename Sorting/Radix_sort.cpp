/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Sorts the given array. Time complexity: O((n+k)*d). Space complexity: O(n+k).
Notes:
       * Given an array, its size, function used to call counting sort it calls the function over 
         the array dim times.
       * Starting from less important because of counting sort stability array will be sorted at the
         end of the algorithm.
===============================================================================*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "Counting_sort.cpp"


template <typename T>
void RadixSort(T *array, int size, int dim, int(*GetIndex)(T, void*))
{
    // Start from less imporant indexes and go to the most important.
    for (int i = 0; i <= dim; ++i)
    {
        // Pass compare index as a void * to the GetIndex function and call counting sort with it.
        int compare_index = i;
        CountingSort(array, size, GetIndex, (void *)(&compare_index));
    }
}   

/*
//============== USAGE EXAMPLE ===================

// Value used to lexicographical sort.
int longest_string_length;

// Value used to get given character in the string. Index is passed by void * because it will be
// given to the radix sort function.
int GetGivenCharacterInString(std::string self, void *character_index)
{
    int idx = (*(int *)character_index);
    if (self.length() <= longest_string_length-idx-1)
        return 0;
    else 
        return (int)self[longest_string_length-idx-1]-(int)'A'+1;
}

// Get given digit with a given index in the number. Index is passed by void * because it will be
// given to the radix sort function.
int GetDigit(int self, void *character_index)
{
    int idx = (*(int *)character_index);
    return ((int)(self/(pow(10, idx))))%10; 
}

int main()
{
    std::string array[] = { "AB", "ABC", "AAA", "ABCD", "BBA", "ABCCDDAA", 
        "AA", "ABCCEACC", "AA", "ABCED", "ABCCDD", "A", "BB" };
    longest_string_length = 8;
    std::cout << "Input tring string array: ";
    for (int i = 0; i < 13; ++i)
        std::cout << array[i].c_str() << " ";
    RadixSort(array, 13, longest_string_length, GetGivenCharacterInString);
    std::cout << "\nSorted (lexicographical): ";
    for (int i = 0; i < 13; ++i)
        std::cout << array[i].c_str() << " ";
    srand(time(NULL));
    int *int_array = new int[10];
    std::cout << "\nInput int array: ";
    for (int i = 0; i < 10; ++i)
        int_array[i] = rand()%1000;
    for (int i = 0; i < 10; ++i)
        std::cout << int_array[i] << " ";
    RadixSort(int_array, 10, 5, GetDigit);
    std::cout << "\nSorted array: ";
    for (int i = 0; i < 10; ++i)
        std::cout << int_array[i] << " ";
    //Output:
    //  Input tring string array: AB ABC AAA ABCD BBA ABCCDDAA AA ABCCEACC AA ABCED ABCCDD A BB
    //  Sorted (lexicographical): A AA AA AAA AB ABC ABCCDD ABCCDDAA ABCCEACC ABCD ABCED BB BBA
    //  Input int array: 920 874 137 403 228 826 757 431 756 851
    //  Sorted array: 137 228 403 431 756 757 826 851 874 920

    return 0;
}
*/