#pragma once

#include <log4cxx/logger.h>
#include <QObject>
#include <QtMultimedia/QtMultimedia>
#include <QUrl>
#include <QMediaPlayer>
#include <QAudioOutput>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    static SoundManager &instance();

    void setVolume(int percent);
    int getVolume() const { return volumePercent; }
    void setEnabled(bool enabled);
    int getEnabled() const { return isEnabled; }

public slots:
    void playNotification();

private:
    explicit SoundManager(QObject *parent = nullptr);

    log4cxx::LoggerPtr logger;
    QPointer<QMediaPlayer> player;
    int volumePercent{75};
    bool isEnabled{};
    QPointer<QAudioOutput> audioOutput;
    QPointer<QMediaPlayer> mediaPlayer;
};
