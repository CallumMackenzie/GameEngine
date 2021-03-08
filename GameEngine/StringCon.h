#pragma once

#include <locale>
#include <iostream>
#include <string>
#include <sstream>

namespace string_conversion {
    inline std::wstring widen(const std::string& str)
    {
        using namespace std;
        wostringstream wstm;
        const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(wstm.getloc());
        for (size_t i = 0; i < str.size(); ++i)
            wstm << ctfacet.widen(str[i]);
        return wstm.str();
    }

    inline std::string narrow(const std::wstring& str)
    {
        using namespace std;
        ostringstream stm;

        const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(stm.getloc());

        for (size_t i = 0; i < str.size(); ++i)
            stm << ctfacet.narrow(str[i], 0);
        return stm.str();
    }
};