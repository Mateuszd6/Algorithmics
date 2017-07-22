/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Naive algorithm for pattern matching. Time complexity: O(n*m). Space complexity: O(1).
Notes:
       * Slowest from all pattern matching algorithms used only to compare better algorithms with it.
       * Tries to adjust pattern at every index of the text.
===============================================================================*/

#include <iostream>
#include <string>
#include <vector>

// Find all indexes of pattern in given text.
std::vector<int> *BruteForcePatternMatching(const std::string &pattern, const std::string &text)
{
    // Vector to store indexes of found matches.
    std::vector <int> *res = new std::vector<int>();

    if (pattern.length() > text.length())
        return res;

    // Try check pattern in every possible index.
    for(int i = 0; i <= text.length()-pattern.length(); ++i)
        for (int j = 0; j < pattern.length(); ++j)
        {
            // Missmatch is found. 
            if (pattern[j] != text[i+j])
                break;

            // If loop has reached it's end without missmatch, add i to the result.
            if (j == pattern.length()-1)
                res->push_back(i);
        }

    return res;
}

/*
//============== USAGE EXAMPLE ===================

int main ()
{
    std::string pattern = "abc";
    std::string text = "ababcaabcabbabcba";
    std::cout << "Text: " << text << "\nPattern: " << pattern << "\n";
    std::vector<int> *matching_result = BruteForcePatternMatching(pattern, text);
    std::cout << "Pattern exist in the text at indexes: ";
    for (auto i : (* matching_result))
        std::cout << i << " ";
    std::cout << "\n";
    //Output:
    //  Text: ababcaabcabbabcba
    //  Pattern: abc
    //  Pattern exist in the text at indexes: 2 6 12

    return 0;
}
*/
