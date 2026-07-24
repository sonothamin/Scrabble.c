#include "sound.h"
#include "raylib.h"
#include <stdbool.h>

typedef struct
{
    bool initialized;

    Music music;
    Sound sfx[SFX_COUNT];

} SoundSystem;

static SoundSystem gSound = {0};

void SoundSysInit(void)
{
    if (gSound.initialized)
        return;

    InitAudioDevice();

    gSound.music = LoadMusicStream("audio/music/background.ogg");

    gSound.sfx[SFX_APP_LAUNCH]        = LoadSound("audio/sfx/app_started.wav");
    gSound.sfx[SFX_ABOUT]        = LoadSound("audio/sfx/about.wav");
    gSound.sfx[SFX_BUTTON]       = LoadSound("resources/audio/sfx/button.wav");
    gSound.sfx[SFX_TILE_PLACE]   = LoadSound("resources/audio/sfx/place.mp3");
    gSound.sfx[SFX_SCORE]        = LoadSound("audio/sfx/earn_point.wav");
    gSound.sfx[SFX_INVALID_MOVE] = LoadSound("audio/sfx/invalid_move.wav");
    gSound.sfx[SFX_GAME_START]   = LoadSound("audio/sfx/game_start.wav");
    gSound.sfx[SFX_GAME_OVER]    = LoadSound("audio/sfx/game_over.wav");
    gSound.sfx[SFX_ERROR]    = LoadSound("audio/sfx/err.wav");
    gSound.sfx[SFX_BACK_NAV]    = LoadSound("audio/sfx/back.wav");
    gSound.sfx[SFX_SCORE_2W]    = LoadSound("audio/sfx/earn_point_bonus_2W.wav");
    gSound.sfx[SFX_SCORE_3W]    = LoadSound("audio/sfx/earn_point_bonus_3W.wav");

    SetMusicVolume(gSound.music, 0.35f);

    for (int i = 0; i < SFX_COUNT; i++)
        SetSoundVolume(gSound.sfx[i], 0.8f);

    PlayMusicStream(gSound.music);

    gSound.initialized = true;
}

void PlaySoundEffect(SoundEffect effect)
{
    if (!gSound.initialized)
        return;

    if (effect >= 0 && effect < SFX_COUNT)
        PlaySound(gSound.sfx[effect]);
}

void SoundSysUpdate(void)
{
    if (gSound.initialized)
        UpdateMusicStream(gSound.music);
}

void SoundSysShutdown(void)
{
    if (!gSound.initialized)
        return;

    StopMusicStream(gSound.music);
    UnloadMusicStream(gSound.music);

    for (int i = 0; i < SFX_COUNT; i++)
        UnloadSound(gSound.sfx[i]);

    CloseAudioDevice();

    gSound.initialized = false;
}

void PauseBackgroundMusic(void)
{
    if (gSound.initialized)
        {
            PauseMusicStream(gSound.music);
            TraceLog(LOG_INFO, "[SOUND] BGM Paused.");
        }
}

void ResumeBackgroundMusic(void)
{
    if (gSound.initialized)
        {
            ResumeMusicStream(gSound.music);
            TraceLog(LOG_INFO, "[SOUND] BGM Resumed");
        }
}

void StopBackgroundMusic(void)
{
    if (gSound.initialized)
        {
            StopMusicStream(gSound.music);
            TraceLog(LOG_INFO, "[SOUND] BGM Stopped.");
        }
}

void SetMusicVolumeLevel(float volume)
{
    if (gSound.initialized)
        {
            SetMusicVolume(gSound.music, volume);
            TraceLog(LOG_INFO, "[SOUND] Music Volume Set to %.2f", volume);
        }
}

void SetSfxVolumeLevel(float volume)
{
    if (!gSound.initialized)
        {
            TraceLog(LOG_ERROR, "[SOUND] Music Volume was attempted to set to %.2f. But Sound Sub-System Not Initialized", volume);
            return;
        }

    for (int i = 0; i < SFX_COUNT; i++)
        {
            SetSoundVolume(gSound.sfx[i], volume);
            TraceLog(LOG_INFO, "[SOUND] SFx Volume [%d] Set to %.2f", i, volume);
        }
}
