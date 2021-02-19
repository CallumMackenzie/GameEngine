// Callum Mackenzie
#include <wincodec.h>
#include <wincodecsdk.h>

#include "C_WICImageFactory.h"

namespace wicin
{
    bool g_WIC2 = false;
    BOOL WINAPI InitializeWICFactory(PINIT_ONCE, PVOID, PVOID *ifactory) noexcept
    {
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8) || defined(_WIN7_PLATFORM_UPDATE)
        HRESULT hr = CoCreateInstance(
            CLSID_WICImagingFactory2,
            nullptr,
            CLSCTX_INPROC_SERVER,
            __uuidof(IWICImagingFactory2),
            ifactory);

        if (SUCCEEDED(hr))
        {
            g_WIC2 = true;
            return TRUE;
        }
        else
        {
            hr = CoCreateInstance(
                CLSID_WICImagingFactory1,
                nullptr,
                CLSCTX_INPROC_SERVER,
                __uuidof(IWICImagingFactory),
                ifactory);
            return SUCCEEDED(hr) ? TRUE : FALSE;
        }
#else
        return SUCCEEDED(CoCreateInstance(
                   CLSID_WICImagingFactory,
                   nullptr,
                   CLSCTX_INPROC_SERVER,
                   __uuidof(IWICImagingFactory),
                   ifactory))
                   ? TRUE
                   : FALSE;
#endif
    }
}

bool C_WICImagingFactory::IsWIC2() noexcept
{
    return wicin::g_WIC2;
}

IWICImagingFactory *C_WICImagingFactory::GetWIC() noexcept
{
    static INIT_ONCE s_initOnce = INIT_ONCE_STATIC_INIT;

    IWICImagingFactory *factory = nullptr;
    if (!InitOnceExecuteOnce(
            &s_initOnce,
            wicin::InitializeWICFactory,
            nullptr,
            reinterpret_cast<LPVOID *>(&factory)))
    {
        return nullptr;
    }
    return factory;
}

IWICImagingFactory *C_WICImagingFactory::WIC()
{
    return GetWIC();
}