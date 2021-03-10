#pragma once

#include <time.h>
#include "GameObject.h"
#include "Direct2D.h"
#include "Log.h"

namespace ingenium2D {

	template <typename T>
	class Renderable : public GameObject
	{
		//TODO : Static assert preventing certain template args
	public:
		inline Renderable(const char* name_, Vector2 pos, Rotation rot, T* rElement) : GameObject(name_, pos, rot)
		{
			renderElement = rElement;
		};

		Renderable(const char* name_, Vector2 pos, Rotation rot)
			: GameObject(name_, pos, rot)
		{ };
		Renderable(const char* name_, Vector2 pos)
			: Renderable(name_, pos, Rotation())
		{ };
		Renderable(const char* name_)
			: Renderable(name_, Vector2(), Rotation())
		{ };
		Renderable(const char* name_, Rotation rot)
			: Renderable(name_, Vector2(), rot)
		{ };

		Renderable(const char* name_, Vector2 pos, T* rElement)
			: Renderable(name_, pos, Rotation(), rElement)
		{ };
		Renderable(const char* name_, T* rElement)
			: Renderable(name_, Vector2(), Rotation(), rElement)
		{ };
		Renderable(const char* name_, Rotation rot, T* rElement)
			: Renderable(name_, Vector2(), rot, rElement)
		{ };

		static inline HRESULT loadFileBitmap(LPCWSTR uri, UINT destinationWidth, UINT destinationHeight, ID2D1Bitmap** ppBitmap, ID2D1RenderTarget* pRT)
		{
			IWICImagingFactory* pIWICFactory = C_WICImagingFactory::GetWIC();
			if (!pIWICFactory) {
				return E_FAIL;
			}
			IWICBitmapDecoder* pDecoder = NULL;
			IWICBitmapFrameDecode* pSource = NULL;
			IWICStream* pStream = NULL;
			IWICFormatConverter* pConverter = NULL;
			IWICBitmapScaler* pScaler = NULL;

			HRESULT hr = pIWICFactory->CreateDecoderFromFilename(
				uri,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&pDecoder
			);
			if (SUCCEEDED(hr))
			{
				hr = pDecoder->GetFrame(0, &pSource);
			}

			if (SUCCEEDED(hr))
			{

				// Convert the image format to 32bppPBGRA
				// (DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED).
				hr = pIWICFactory->CreateFormatConverter(&pConverter);
			}

			if (SUCCEEDED(hr))
			{
				hr = pConverter->Initialize(
					pSource,
					GUID_WICPixelFormat32bppPBGRA,
					WICBitmapDitherTypeNone,
					NULL,
					0.f,
					WICBitmapPaletteTypeMedianCut
				);
			}

			if (SUCCEEDED(hr))
			{
				// Create a Direct2D bitmap from the WIC bitmap.
				hr = pRT->CreateBitmapFromWicBitmap(
					pConverter,
					NULL,
					ppBitmap
				);
			}

			if (pDecoder) {
				pDecoder->Release();
				pDecoder = NULL;
			}
			if (pSource) {
				pSource->Release();
				pSource = NULL;
			}
			if (pStream) {
				pStream->Release();
				pStream = NULL;
			}
			if (pConverter) {
				pConverter->Release();
				pConverter = NULL;
			}
			if (pScaler) {
				pScaler->Release();
				pScaler = NULL;
			}
			return hr;
		}

	public:
		struct FrameData {
			static const bool SPRITESHEET_VERTICAL = true;
			static const bool SPRITESHEET_HORIZONTAL = false;

			int frames = 1;
			int frame = 0;
			float frameTime = 0;
			int frameWidth = 0;
			int frameHeight = 0;
			clock_t frameDeltaClock = clock();
			bool spriteSheetDirection = SPRITESHEET_HORIZONTAL;

			void calculateFrame() {
				if (((float)clock() / CLOCKS_PER_SEC) - ((float)frameDeltaClock / CLOCKS_PER_SEC) >= frameTime) {
					frameDeltaClock = clock();
					frame++;
					if (frame >= frames) {
						frame = 0;
					}
				}
			}
		};

		T* renderElement = nullptr;
		float transparency = 1.f;
		FrameData frameData = FrameData();
		// int renderIndex = 0;
	};
};