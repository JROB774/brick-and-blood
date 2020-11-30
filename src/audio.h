#ifndef AUDIO_H
#define AUDIO_H

typedef enum SoundID__
{
    // Nothing...
    SND_TOTAL

} SoundID;

typedef enum MusicID__
{
    // Nothing...
    MUS_TOTAL

} MusicID;

INTERNAL void init_audio ();
INTERNAL void quit_audio ();

INTERNAL int  play_sound         (SoundID id, int loops);
INTERNAL int  play_sound_channel (SoundID id, int loops, int channel);
INTERNAL void play_music         (MusicID id, int loops);
INTERNAL void stop_channel       (int channel);

// Audio volume should always be set with these functions rather than setting
// the gAudio.sound_volume and gAudio.music_volume values directly. As these
// functions performs some extra operations in order for the volume to change.
INTERNAL void  set_sound_volume (float volume); // [0-1]
INTERNAL void  set_music_volume (float volume); // [0-1]
INTERNAL float get_sound_volume ();
INTERNAL float get_music_volume ();

// SOUND GROUPS

// Nothing...

// MUSIC GROUPS

// Nothing...

#endif /* AUDIO_H */
