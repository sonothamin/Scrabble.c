// sound.h

#ifndef SOUND_H
#define SOUND_H

typedef enum
{
    SFX_BUTTON,
    SFX_TILE_PLACE,
    SFX_SCORE,
    SFX_SCORE_2W,
    SFX_SCORE_3W,
    SFX_ERROR,
    SFX_BACK_NAV,
    SFX_INVALID_MOVE,
    SFX_GAME_START,
    SFX_GAME_OVER,

    SFX_COUNT
} SoundEffect;

void SoundSysInit(void);
void SoundSysUpdate(void);
void SoundSysShutdown(void);

void PlaySoundEffect(SoundEffect effect);

void PauseBackgroundMusic(void);
void ResumeBackgroundMusic(void);
void StopBackgroundMusic(void);

void SetMusicVolumeLevel(float volume);
void SetSfxVolumeLevel(float volume);

#endif