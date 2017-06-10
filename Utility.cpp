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
}