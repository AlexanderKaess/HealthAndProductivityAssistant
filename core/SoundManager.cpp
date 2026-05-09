#include "SoundManager.h"

SoundManager::SoundManager(QObject *parent) : QObject(parent) {}

SoundManager &SoundManager::instance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::setVolume(int percent) {
    volumePercent = qBound(0, percent, 100);
}

void SoundManager::setEnabled(bool enabled) {
    isEnabled = enabled;
}

void SoundManager::playNotification() {
    if(!isEnabled){
        return;
    }

    system("afplay /System/Library/Sounds/Ping.aiff &");
}

