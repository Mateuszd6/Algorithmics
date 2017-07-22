/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Rabin-Karp pattern matching algorithm. Time complexity: O(n+m). Space complexity: O(n+m).
Notes:
       * Works in linear time but with worse space complexity than Bayer-Moore algorithm.
       * The constant value hidden in the time complexity is quite big so this algorithm works slower than KMP.
       * Theoretically it is possible for this algorithm to output wrong answer.
===============================================================================*/

#include <iostream>
#include <string>
#include <vector>

// Find all indexes of pattern in given text.
std::vector<int> *RabinKarp(const std::string &pattern, const std::string &text)
{
    // Vector to store indexes of found matches.
    std::vector <int> *res = new std::vector<int>();

    if (pattern.length() > text.length())
        return res;

    // Values used for hashing.
    int p = 37;

    unsigned long long pattern_hash = 0, interval_hash = 0;
    
    // Vector storing the powers of p;
    std::vector < unsigned long long > p_pow(text.length()+1);

    // Fill the powers vector.
    p_pow[0] = 1;
    for (int i = 1; i <= text.length(); ++i)
        p_pow[i] = p_pow[i-1]*p;

    // Calulate the pattern hash and the hash of the first text interval of the pattern length.
    for(int i = 0; i < pattern.length(); ++i)
    {
        pattern_hash += ((unsigned long long)(pattern[i]-'a'+1))*p_pow[i];
        interval_hash += ((unsigned long long)(text[i]-'a'+1))*p_pow[i];
    }

    for(int i = 0; i < text.length()-pattern.length()+1; ++i)
    {
        // If hashes are same, probobly substrings are same too.
        if (pattern_hash == interval_hash)
            res->push_back(i);
            
        // If end is not reached recalculate hashes.
        if (i != text.length()-pattern.length()+1)
        {
            interval_hash = interval_hash - ((text[i]-'a'+1)*p_pow[i]);
            interval_hash = interval_hash + (text[i+pattern.length()]-'a'+1)*p_pow[i+pattern.length()];
            pattern_hash = pattern_hash*p;
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
    std::vector<int> *matching_result = RabinKarp(pattern, text);
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