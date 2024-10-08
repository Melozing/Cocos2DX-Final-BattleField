#ifndef __MUSIC_EVENT_H__
#define __MUSIC_EVENT_H__

enum class MusicEventType {
    BEAT,
    KICK,
    SNARE,
    MELODY,
    LOW,
    MID,
    HIGH,
    DROP,
    RISE,
    CLAP,
    HAT,
    BASS,
    VOCAL,
    SYNTH,
    PAD,
    FX,
    PERCUSSION
};


class MusicEvent {
public:
    MusicEvent(MusicEventType type, float timestamp);

    MusicEventType getType() const;
    float getTimestamp() const;

private:
    MusicEventType type;
    float timestamp;
};

#endif // __MUSIC_EVENT_H__
