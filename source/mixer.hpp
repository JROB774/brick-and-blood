#ifndef MIXER_HPP
#define MIXER_HPP

static struct Mixer
{
    float sound_volume;
    float music_volume;

} gMixer;

struct Sound
{
    Mix_Chunk* data;
};

struct Music
{
    Mix_Music* data;
};

INTERNAL bool InitMixer ();
INTERNAL void QuitMixer ();

INTERNAL void LoadSound (Sound& sound, std::string file_name);
INTERNAL void FreeSound (Sound& sound);
INTERNAL void PlaySound (Sound& sound, int loops =  0);

INTERNAL void LoadMusic (Music& music, std::string file_name);
INTERNAL void FreeMusic (Music& music);
INTERNAL void PlayMusic (Music& music, int loops = -1);

INTERNAL void SetSoundVolume (float volume); // [0-1]
INTERNAL void SetMusicVolume (float volume); // [0-1]

INTERNAL float GetSoundVolume ();
INTERNAL float GetMusicVolume ();

INTERNAL bool IsSoundOn ();
INTERNAL bool IsMusicOn ();

INTERNAL bool IsMusicPlaying ();

#endif /* MIXER_HPP */
