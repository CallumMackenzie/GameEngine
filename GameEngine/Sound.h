#pragma once

// Callum Mackenzie

struct Sound
{
    static void PlaySoundResource(LPCWSTR resource, HINSTANCE hInstance);
    static void PlaySoundResourceLooped(LPCWSTR resource, HINSTANCE hInstance);
    static bool StopAll();
};