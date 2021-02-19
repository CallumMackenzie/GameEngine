// Callum Mackenzie

#include "Windows.h"
#pragma comment(lib, "Winmm.lib")

#include "Sound.h"

void Sound::PlaySoundResource(LPCWSTR resource, HINSTANCE hInstance)
{
    PlaySound(resource, hInstance, SND_RESOURCE | SND_ASYNC);
}

void Sound::PlaySoundResourceLooped(LPCWSTR resource, HINSTANCE hInstance)
{
    PlaySound(resource, hInstance, SND_RESOURCE | SND_ASYNC | SND_LOOP);
}

bool Sound::StopAll()
{
    return PlaySound(NULL, 0, 0);
}
