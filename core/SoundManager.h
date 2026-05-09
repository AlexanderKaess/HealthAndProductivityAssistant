#pragma once

#include <QObject>
#include <QtMultimedia/QtMultimedia>

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
    QPointer<QMediaPlayer> player;
    int volumePercent{75};
    bool isEnabled{};
};
