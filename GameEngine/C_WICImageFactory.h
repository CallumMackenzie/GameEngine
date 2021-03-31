#pragma once

// Callum Mackenzie

#ifdef _WIN32
#include <wincodec.h>
#include <wincodecsdk.h>

namespace wicin
{
    
}

class C_WICImagingFactory
{
public:
    static IWICImagingFactory *WIC();
    static IWICImagingFactory *GetWIC() noexcept;
    static bool IsWIC2() noexcept;
};
#endif