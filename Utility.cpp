/*===============================================================================
Author: Mateusz Dudziński.
Email: mateuszpd7@gmail.com
Full Repository: github.com/Mateuszd6/Algorithmics
Description: Namespace containing some usefull functions.
===============================================================================*/

namespace utility
{
    // Function used to compare template objects, using only '<' operator:
    template <typename T>
	inline bool IsLower(T self, T other) { return self < other; }
    template <typename T>
    inline bool IsEqual(T self, T other) { return (!(self < other) && !(other < self)); }
    template <typename T>
    inline bool IsGreater(T self, T other) { return (!IsEqual(self, other) && !(self < other)); }
    template <typename T>
    inline bool IsGreaterorEqual(T self, T other) { return (!Isless(self, other)); }
    template <typename T>
    inline bool IsLowerOrEqual(T self, T other) { return (!IsGreater(self, other)); }

    // Function used to compare template objects, with generic compare function:
    template <typename T>
	inline bool IsLower(T self, T other, bool(*_compare)(T, T)) 
    { 
        return _compare(self, other); 
    }
    template <typename T>
    inline bool IsEqual(T self, T other, bool(*_compare)(T, T)) 
    {
         return (!(_compare(self, other)) && !(_compare(other, self))); 
    }
    template <typename T>
    inline bool IsGreater(T self, T other, bool(*_compare)(T, T))
    { 
        return (!IsEqual(self, other, _compare) && !(_compare(self, other))); 
    }
    template <typename T>
    inline bool IsGreaterorEqual(T self, T other, bool(*_compare)(T, T)) 
    { 
        return (!Isless(self, other, _compare)); 
    }
    template <typename T>
    inline bool IsLowerOrEqual(T self, T other, bool(*_compare)(T, T)) 
    { 
        return (!IsGreater(self, other, _compare)); 
    }

    // Functions for getting the minimum and maximum from to given objects, using only '<' operator: 
    template <typename T>
    inline T Max(T self, T other) {  return IsGreater(self, other) ? self : other; }
    template <typename T>
    inline T Min(T self, T other) {  return IsLower(self, other) ? self : other; }

    // Returns true if given number is a power of 2.
    inline bool Is2Pow(int n) { return (((n&(n-1))&n) == n); }

    inline int GetHighestBit (int n)
    {
        if (n == 0)
            return -1;
        int res = 0;
        while (n)
        {
            n >>= 1;
            ++res;
        }    
        return res;
    }
    inline int GetLowestBit(int n)
    {
        for (int j = 0; (1 << j) <= n; ++j)
        {
            if ((1 << j) & n)
                return j;
        }

        // This will be returned if 0 is passed.
        return -1;
    }
    inline int GetNumberOfSetBits(int n)
    {
        int res = 0;
        while (n)
        {
            if (n & 1)
                ++res;
            n >>= 1;
        }    
        return res;
    }
}