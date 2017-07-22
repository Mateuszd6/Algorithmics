/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Knuth–Morris–Pratt pattern matching algorithm. Time complexity: O(n+m). Space complexity: O(n+m).
Notes:
       * Works very fast and always outputs correct answer.
       * It builds a prefix array used to speed up pattern searching.
===============================================================================*/

#include <iostream>
#include <string>
#include <vector>

// Find all indexes of pattern in given text.
std::vector<int> *KMPSubstringMatching(const std::string &pattern, const std::string &text)
{
    // Vector to store indexes of found matches.
    std::vector <int> *res = new std::vector<int>();

    if (pattern.length() > text.length())
        return res;

    // NOTE: 's' and '#' are unique characters that do not exist in either the text or pattern.
    std::string w = '$' + pattern + '#' + text;

    // Prefix array.
    std::vector < int > pref(pattern.length()+text.length()+4);
    pref[0] = 0;
    pref[1] = 0;


    for(int i = 2; i < w.length(); ++i)
    {
        int j = pref[i-1];

        // While missmatch jump on the prefix array to find a prefix's prefix which does not generate collision.
        // It is possible that this prefix is an empty word. 
        while (j != 0 && w[j+1] != w[i])
            j = pref[j];
        
        // No prefix but first character matches.
        if (j == 0 && w[1] == w[i])
            pref[i] = 1;
        
        // No prefix and first character does not match.
        else if (j ==0)
            pref[i] = 0;
        
        // There is prefix. The match is being extended. 
        else
            pref[i] = j+1;
    }

    // Basically the match is prefix of patterns length. Search prefix array to find such occurrences.
    for(int i = 1; i < w.length(); ++i)
        if(pref[i] == pattern.length())
            res->push_back(i- 2*pattern.length()-1);

    return res;
}

/*
//============== USAGE EXAMPLE ===================

int main ()
{
    std::string pattern = "abc";
    std::string text = "ababcaabcabbabcba";
    std::cout << "Text: " << text << "\nPattern: " << pattern << "\n";
    std::vector<int> *matching_result = KMPSubstringMatching(pattern, text);
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