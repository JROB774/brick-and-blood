GLOBAL constexpr int MIXER_FREQUENCY     = MIX_DEFAULT_FREQUENCY;
GLOBAL constexpr U16 MIXER_SAMPLE_FORMAT = MIX_DEFAULT_FORMAT;
GLOBAL constexpr int MIXER_CHANNELS      = 2; // Stereo Sound
GLOBAL constexpr int MIXER_SAMPLE_SIZE   = 2048;

INTERNAL void LoadSound (Sound& sound, std::string file_name)
{
    sound.data = Mix_LoadWAV(file_name.c_str());
    if (!sound.data)
    {
        LOG_ERROR(ERR_MAX, "Failed to load sound '%s'! (%s)", file_name.c_str(), Mix_GetError());
    }
}

INTERNAL void FreeSound (Sound& sound)
{
    Mix_FreeChunk(sound.data);
    sound.data = NULL;
}

INTERNAL void LoadMusic (Music& music, std::string file_name)
{
    music.data = Mix_LoadMUS(file_name.c_str());
    if (!music.data)
    {
        LOG_ERROR(ERR_MAX, "Failed to load music '%s'! (%s)", file_name.c_str(), Mix_GetError());
    }
}

INTERNAL void FreeMusic (Music& music)
{
    Mix_FreeMusic(music.data);
    music.data = NULL;
}

//
// INTERFACE
//

INTERNAL bool InitMixer ()
{
    // Doesn't seem like it's necessary in web builds as the OGG files play anyway even
    // though this function fails when ran on the web? So I guess we'll just disable it.
    #if defined(PLATFORM_WIN32)
    if (!(Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG))
    {
        LOG_ERROR(ERR_MAX, "Failed to initialize mixer OGG functionality! (%s)", Mix_GetError());
    }
    #endif // PLATFORM_WIN32

    if (Mix_OpenAudio(MIXER_FREQUENCY, MIXER_SAMPLE_FORMAT, MIXER_CHANNELS, MIXER_SAMPLE_SIZE) != 0)
    {
        LOG_ERROR(ERR_MAX, "Failed to open audio device! (%s)", Mix_GetError());
        return false;
    }

    Mix_AllocateChannels(32);

    SetSoundVolume(gSettings.sound_volume);
    SetMusicVolume(gSettings.music_volume);

    return true;
}

INTERNAL void QuitMixer ()
{
    Mix_CloseAudio();
}

INTERNAL void PlaySound (std::string sound_name, int loops)
{
    Sound* sound = GetAsset<AssetSound>(sound_name);
    if (!sound) return;
    if (Mix_PlayChannel(-1, sound->data, loops) == -1)
    {
        LOG_ERROR(ERR_MIN, "Failed to play sound effect! (%s)", Mix_GetError());
    }
}

INTERNAL void PlayMusic (std::string music_name, int loops)
{
    Music* music = GetAsset<AssetMusic>(music_name);
    if (!music) return;
    if (Mix_PlayMusic(music->data, loops) == -1)
    {
        LOG_ERROR(ERR_MIN, "Failed to play music! (%s)", Mix_GetError());
    }
}

INTERNAL void SetSoundVolume (float volume)
{
    gMixer.sound_volume = std::clamp(volume, 0.0f, 1.0f);
    int ivolume = (int)((float)MIX_MAX_VOLUME * gMixer.sound_volume);
    Mix_Volume(-1, ivolume);
}

INTERNAL void SetMusicVolume (float volume)
{
    gMixer.music_volume = std::clamp(volume, 0.0f, 1.0f);
    int ivolume = (int)((float)MIX_MAX_VOLUME * gMixer.music_volume);
    Mix_VolumeMusic(ivolume);
}

INTERNAL float GetSoundVolume ()
{
    return gMixer.sound_volume;
}

INTERNAL float GetMusicVolume ()
{
    return gMixer.music_volume;
}

INTERNAL bool IsSoundOn ()
{
    return (gMixer.sound_volume > 0.0f);
}

INTERNAL bool IsMusicOn ()
{
    return (gMixer.music_volume > 0.0f);
}

INTERNAL bool IsMusicPlaying ()
{
    return Mix_PlayingMusic();
}
