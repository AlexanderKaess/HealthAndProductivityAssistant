#pragma once

class ISoundManager{
public:
    virtual ~ISoundManager() = default;
    virtual void setVolume(int percent) = 0;
    virtual void setEnabled(bool enabled) = 0;
    virtual void playNotification() = 0;
};
