/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: KMR pattern matching algorithm. Time complexity: O((n+m)*log(n+m)). Space complexity: O(n+m).
Notes:
       * Not so fast as Rabin-Karp algorithm, but answer is always corrent.
       * Also in can be modified for substring comparation as other pattern matching algorithms cannot.
===============================================================================*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

namespace detail
{
    // Calculate the long long key from two ints and check for it in the hash map.
    // If value does not exist use next_index to add it to the map.
    int KMRGetKey(int a, int b, std::unordered_map<unsigned long long, int> &pair_to_index, int &next_index)
    {
        // u long long is callculated from two ints.
        unsigned long long key = (((unsigned long long)a) << 32) + b;
        auto iter = pair_to_index.find(key);
        
        // There is no such key in a map. Add it.
        if (iter == pair_to_index.end())
        {
            pair_to_index.insert(std::make_pair(key, next_index++));
            return next_index-1;
        }

        // The key is in the map.
        else
            return iter->second;
    }
}

// Find all indexes of pattern in given text.
std::vector<int> *KMRPatternMatching(const std::string &pattern, const std::string &text)
{
    // Vector to store indexes of found matches.
    std::vector <int> *res = new std::vector<int>();

    // Map to store the pair indexes.
    std::unordered_map <unsigned long long, int> pair_to_index;

    if (pattern.length() > text.length())
        return res;

    int pow2_greater_equal_text_size = 0, pow2_greater_equal_patt_size = 0;
    while ((1 << pow2_greater_equal_text_size) < text.length())
        ++pow2_greater_equal_text_size;
    while ((1 << pow2_greater_equal_patt_size) < pattern.length())
        ++pow2_greater_equal_patt_size;

    // These vectors will store text and pattern keys.
    std::vector<int> text_keys(1 << pow2_greater_equal_text_size);
    std::vector<int> pattern_keys(1 << pow2_greater_equal_patt_size);

    // These will be used to compute the new ones.
    std::vector<int> new_text_keys;
    std::vector<int> new_pattern_keys;

    // Inicialize keys vectors.
    for (int i = 0; i < text.length(); ++i)
        text_keys[i] = (int)text[i];
    for (int i = text.length(); i < text_keys.size(); ++i)
        text_keys[i] = 0;
    for (int i = 0; i < pattern.length(); ++i)
        pattern_keys[i] = (int)pattern[i];
    for (int i = pattern.length(); i < pattern_keys.size(); ++i)
        pattern_keys[i] = 0;

    // Threre is no need to compute this to the end. It can be done to the level when there are two comparations
    // for the pattern.
    int lvl = 0;
    while ((1 << (lvl+1)) < pattern.length())
    {
        lvl++;

        // Clear used variables. 
        int next_index = 0;
        pair_to_index.clear();
        new_text_keys.clear();
        new_pattern_keys.clear();
        
        // Compute new vectors using KMRGetKey function.
        for (int i = 0; i + (1 << (lvl-1)) < text_keys.size(); ++i)
            new_text_keys.push_back(detail::KMRGetKey(text_keys[i], text_keys[i+ (1 << (lvl-1))], 
                pair_to_index, next_index));
        for (int i = 0; i + (1 << (lvl-1)) < pattern_keys.size(); ++i)
            new_pattern_keys.push_back(detail::KMRGetKey(pattern_keys[i], pattern_keys[i+ (1 << (lvl-1))], 
                pair_to_index, next_index));

        // Move old ones to the new ones.
        text_keys.clear();
        text_keys = new_text_keys;
        pattern_keys.clear();
        pattern_keys = new_pattern_keys;
    }

    int pow2_less_equal_patt_size = pow2_greater_equal_patt_size;
    if ((1 << pow2_greater_equal_patt_size) >= pattern.size())
        pow2_less_equal_patt_size--;

    // The difference between first and second pattern key to compare with text keys.
    int offset = pattern.length() - (1 << pow2_less_equal_patt_size);

    // Search for ocurrences based on calculated offset.
    for (int i = 0; i +offset < text_keys.size(); ++i)
        if (text_keys[i] == pattern_keys[0] && text_keys[i+offset] == pattern_keys[offset])
            res->push_back(i);
    
    return res;
}

/*
//============== USAGE EXAMPLE ===================

int main ()
{
    std::string pattern = "abca";
    std::string text = "ababcabcabbabcaa";
    std::cout << "Text: " << text << "\nPattern: " << pattern << "\n";
    std::vector<int> *matching_result = KMRPatternMatching(pattern, text);
    std::cout << "Pattern exist in the text at indexes: ";
    for (auto i : (* matching_result))
        std::cout << i << " ";
    std::cout << "\n";
    //Output:
    //  Text: ababcabcabbabcaa
    //  Pattern: abca
    //  Pattern exist in the text at indexes: 2 5 11

    return 0;
}
*/