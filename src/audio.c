////////////////////////////////////////////////////////////////////////////////

// The stuff in this section is all used internally by the audio/sound system!

#define AUDIO_FREQUENCY      MIX_DEFAULT_FREQUENCY
#define AUDIO_SAMPLE_FORMAT  MIX_DEFAULT_FORMAT
#define AUDIO_CHANNELS       2 // Stereo Sound
#define AUDIO_SAMPLE_SIZE    2048
#define AUDIO_SOUND_CHANNELS 64

typedef struct Sound__ { Mix_Chunk* data; } Sound;
typedef struct Music__ { Mix_Music* data; } Music;

GLOBAL struct
{
    Sound sound[SND_TOTAL];
    Music music[MUS_TOTAL];

    bool initialized;

    float sound_volume;
    float music_volume;

} gAudio;

INTERNAL void load_sound (Sound* sound, const char* file_name)
{
    assert(sound);
    if (gAudio.initialized)
    {
        sound->data = Mix_LoadWAV(file_name);
        if (!sound->data)
        {
            LOGDEBUG("Failed to load sound \"%s\"! (%s)", file_name, Mix_GetError());
        }
    }
}

INTERNAL void free_sound (Sound* sound)
{
    assert(sound);
    Mix_FreeChunk(sound->data);
    sound->data = NULL;
}

INTERNAL void load_music (Music* music, const char* file_name)
{
    assert(music);
    if (gAudio.initialized)
    {
        music->data = Mix_LoadMUS(file_name);
        if (!music->data)
        {
            LOGDEBUG("Failed to load music \"%s\"! (%s)", file_name, Mix_GetError());
        }
    }
}

INTERNAL void free_music (Music* music)
{
    assert(music);
    Mix_FreeMusic(music->data);
    music->data = NULL;
}

////////////////////////////////////////////////////////////////////////////////

INTERNAL void init_audio ()
{
    // Doesn't seem like it's necessary in web builds as the OGG files play anyway even
    // though this function fails when ran on the web? So I guess we'll just disable it.
    #if defined(PLATFORM_WIN32)
    if (!(Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG))
    {
        LOGWARNING("Failed to initialize mixer OGG functionality! (%s)", Mix_GetError());
    }
    #endif

    if (Mix_OpenAudio(AUDIO_FREQUENCY, AUDIO_SAMPLE_FORMAT, AUDIO_CHANNELS, AUDIO_SAMPLE_SIZE) != 0)
    {
        LOGWARNING("Failed to open audio device! (%s)", Mix_GetError());
    }
    else
    {
        Mix_AllocateChannels(AUDIO_SOUND_CHANNELS);

        set_sound_volume(CAST(float,gSettings.sound_volume)/CAST(float,MIX_MAX_VOLUME));
        set_music_volume(CAST(float,gSettings.music_volume)/CAST(float,MIX_MAX_VOLUME));

        gAudio.initialized = true;

        // Load all of the sounds.
        // ...
        // Load all of the music.
        // ...
    }
}

INTERNAL void quit_audio ()
{
    if (gAudio.initialized)
    {
        // Free all of the sounds.
        // ...
        // Free all of the music.
        // ...

        Mix_CloseAudio();
    }

    Mix_Quit();
}

INTERNAL int play_sound (SoundID id, int loops)
{
    return play_sound_channel(id,loops,-1);
}
INTERNAL int play_sound_channel (SoundID id, int loops, int channel)
{
    assert((id >= 0) && (id < SND_TOTAL));
    int returned_channel = 0;
    if (gAudio.initialized)
    {
        returned_channel = Mix_PlayChannel(channel, gAudio.sound[id].data, loops);
        if (returned_channel == -1)
        {
            LOGDEBUG("Failed to play sound! (%s)", Mix_GetError());
        }
    }
    return returned_channel;
}

INTERNAL void play_music (MusicID id, int loops)
{
    assert((id >= 0) && (id < MUS_TOTAL));
    if (gAudio.initialized)
    {
        if (Mix_PlayMusic(gAudio.music[id].data, loops) == -1)
        {
            LOGDEBUG("Failed to play music! (%s)", Mix_GetError());
        }
    }
}

INTERNAL void stop_channel (int channel)
{
    Mix_HaltChannel(channel);
}

INTERNAL void set_sound_volume (float volume)
{
    volume = CLAMP(volume, 0.0f, 1.0f);
    gAudio.sound_volume = CLAMP(volume, 0.0f, 1.0f);
    int ivolume = CAST(int, CAST(float,MIX_MAX_VOLUME)*gAudio.sound_volume);
    Mix_Volume(-1, ivolume);
}
INTERNAL void set_music_volume (float volume)
{
    volume = CLAMP(volume, 0.0f, 1.0f);
    gAudio.music_volume = CLAMP(volume, 0.0f, 1.0f);
    int ivolume = CAST(int, CAST(float,MIX_MAX_VOLUME)*gAudio.music_volume);
    Mix_VolumeMusic(ivolume);
}

INTERNAL float get_sound_volume ()
{
    return gAudio.sound_volume;
}
INTERNAL float get_music_volume ()
{
    return gAudio.music_volume;
}
