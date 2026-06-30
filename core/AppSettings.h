#pragma once

#include <QObject>
#include <QSettings>

class AppSettings : public QObject
{
    Q_OBJECT

public:
    explicit AppSettings(QObject* parent = nullptr);

    virtual bool getSoundEnabled() const { return soundEnabled; }
    virtual bool getPopupEnabled() const { return popupEnabled; }
    virtual bool getConfirmClose() const { return confirmClose; }
    virtual int getVolume() const { return volume; }
    virtual int getThemeIndex() const { return themeIndex; }
    virtual int getLanguageIndex() const { return languageIndex; }

    virtual void setSoundEnabled(bool enabled);
    virtual void setPopupEnabled(bool enabled);
    virtual void setConfirmClose(bool confirm);
    virtual void setVolume(int volume);
    virtual void setThemeIndex(int index);
    virtual void setLanguageIndex(int language);

    virtual void load();
    virtual void save() const;
    virtual void reset();

signals:
    void soundEnabledChanged(bool enabled);
    void popupEnabledChanged(bool enabled);
    void confirmCloseChanged(bool confirm);
    void volumeChanged(int volumeValue);
    void themeIndexChanged(int index);
    void languageIndexChanged(int lanuage);

private:
    bool soundEnabled{true};
    bool popupEnabled{true};
    bool confirmClose{true};
    int volume{75};
    int themeIndex{0};
    int languageIndex{0};
};
