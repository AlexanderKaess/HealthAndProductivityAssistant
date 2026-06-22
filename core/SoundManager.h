#pragma once

#include <QAudioOutput>
#include <QMediaPlayer>
#include <QObject>
#include <QUrl>
#include <QtMultimedia/QtMultimedia>
#include <log4cxx/logger.h>

#include "ISoundManager.h"

class SoundManager : public QObject, public ISoundManager
{
    Q_OBJECT
public:
    static SoundManager &instance();

    void setVolume(int percent) override;
    int getVolume() const { return volumePercent; }
    void setEnabled(bool enabled) override;
    int getEnabled() const { return isEnabled; }

public slots:
    void playNotification() override;

private:
    explicit SoundManager(QObject *parent = nullptr);

    log4cxx::LoggerPtr logger;
    QPointer<QMediaPlayer> player;
    int volumePercent{75};
    bool isEnabled{};
    QPointer<QAudioOutput> audioOutput;
    QPointer<QMediaPlayer> mediaPlayer;
};
