#pragma once

#include <QObject>
#include <QSettings>

class AppSettings : public QObject
{
    Q_OBJECT

public:
    explicit AppSettings(QObject* parent = nullptr);

    bool getSoundEnabled() const { return soundEnabled; }
    bool getPopupEnabled() const { return popupEnabled; }
    bool getConfirmClose() const { return confirmClose; }
    int  getVolume() const { return volume; }
    int  getThemeIndex() const { return themeIndex; }
    int  getLanguageIndex() const { return languageIndex; }

    void setSoundEnabled(bool enabled);
    void setPopupEnabled(bool enabled);
    void setConfirmClose(bool confirm);
    void setVolume(int volume);
    void setThemeIndex(int index);
    void setLanguageIndex(int language);

    void load();
    void save() const;
    void reset();

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
