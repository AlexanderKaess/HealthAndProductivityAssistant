#include "SoundManager.h"

SoundManager::SoundManager(QObject *parent) :
    QObject(parent)
    , logger(log4cxx::Logger::getLogger("HealthLogger.SoundManager"))
{
    //load sound file
    QString soundPath = "qrc:/sounds/message-notification-4.wav";
    if(!QFile::exists(soundPath)){
        LOG4CXX_INFO(logger, "Error while loading sound file");
    }

    audioOutput = new QAudioOutput(this);
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(soundPath);

    audioOutput->setVolume(volumePercent / 100.0);

    if (!mediaPlayer->errorString().isEmpty()) {
        LOG4CXX_INFO(logger, "Mediaplayer not ready. Status: "  + mediaPlayer->errorString().toStdString());
    }
}

SoundManager& SoundManager::instance() {
    static SoundManager instance;
    return instance;
}

void SoundManager::setVolume(int percent) {
    volumePercent = qBound(0, percent, 100);
    audioOutput->setVolume(volumePercent / 100.0f);
}

void SoundManager::setEnabled(bool enabled) {
    isEnabled = enabled;
}

void SoundManager::playNotification() {
    if(!isEnabled){
        LOG4CXX_INFO(logger, "Mediaplayer do not play notification");
        return;
    }

    LOG4CXX_INFO(logger, "Mediaplayer play notification");
    mediaPlayer->play();
}

