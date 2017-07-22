/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Bayer-Moore pattern matching algorithm. Time complexity: O(n*m). Space complexity: O(1).
Notes:
       * In most cases this algorithm works faster than the standard brute force approach.
===============================================================================*/

#include <iostream>
#include <string>
#include <vector>

// Find all indexes of pattern in given text.
std::vector<int> *BayerMoorePatternMatching(const std::string &pattern, const std::string &text)
{
    // Vector to store indexes of found matches.
    std::vector <int> *res = new std::vector<int>();

    if (pattern.length() > text.length())
        return res;

    // How much first pattern index is shifted to the begginig of the text.
    int shift = 0;

    while (shift + pattern.length() <= text.length())
    {
        // Iterate similarly to the brute force algorithm.
        for(int i = pattern.length()-1; i >= 0; --i)
        {
            // If it is impossible to make a match...
            if (pattern[i] != text[i+shift])
            {
                // Try to shift to the right until match happens or the text ends.
                while (pattern[i] != text[i+shift] && shift <= text.length() - pattern.length()+1)
                    ++shift;
                break;
            }
            
            // Full pattern is matched, loop has reached it's end.
            if (i == 0)
            {
                res->push_back(shift);
                shift++;
            }
        }
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
    std::vector<int> *matching_result = BayerMoorePatternMatching(pattern, text);
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