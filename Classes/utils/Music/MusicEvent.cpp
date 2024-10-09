#include "MusicEvent.h"

MusicEvent::MusicEvent(MusicEventType type, float timestamp)
    : type(type), timestamp(timestamp) {}

MusicEventType MusicEvent::getType() const {
    return type;
}

float MusicEvent::getTimestamp() const {
    return timestamp;
}
