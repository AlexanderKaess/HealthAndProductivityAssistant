#include "AppSettings.h"

AppSettings::AppSettings(QObject* parent)
    : QObject(parent){}

void AppSettings::setSoundEnabled(bool enabled) {
    if (soundEnabled == enabled) {
        return;
    }

    soundEnabled = enabled;
    emit soundEnabledChanged(enabled);
}

void AppSettings::setPopupEnabled(bool enabled) {
    if (popupEnabled == enabled) {
        return;
    }

    popupEnabled = enabled;
    emit popupEnabledChanged(enabled);
}

void AppSettings::setConfirmClose(bool confirm) {
    if (confirmClose == confirm) {
        return;
    }

    confirmClose = confirm;
    emit confirmCloseChanged(confirm);
}

void AppSettings::setVolume(int volumeValue) {
    volumeValue = qBound(0, volumeValue, 100);
    if (volume == volumeValue) {
        return;
    }

    volume = volumeValue;
    emit volumeChanged(volumeValue);
}

void AppSettings::setThemeIndex(int index) {
    if (themeIndex == index) {
        return;
    }

    themeIndex = index;
    emit themeIndexChanged(index);
}

void AppSettings::setLanguageIndex(int language) {
    if (languageIndex == language) {
        return;
    }

    languageIndex = language;
    emit languageIndexChanged(language);
}

void AppSettings::load() {
    QSettings s("MyCompany", "HealthProductivityApp");
    soundEnabled  = s.value("sound", true).toBool();
    popupEnabled  = s.value("popup", true).toBool();
    volume        = s.value("volume", 75).toInt();
    themeIndex    = s.value("theme", 0).toInt();
    languageIndex = s.value("language", 0).toInt();
    confirmClose  = s.value("confirmClose", true).toBool();
}

void AppSettings::save() const {
    QSettings s("MyCompany", "HealthProductivityApp");
    s.setValue("sound", soundEnabled);
    s.setValue("popup", popupEnabled);
    s.setValue("volume", volume);
    s.setValue("theme", themeIndex);
    s.setValue("language", languageIndex);
    s.setValue("confirmClose", confirmClose);
}

void AppSettings::reset() {
    setSoundEnabled(true);
    setPopupEnabled(true);
    setConfirmClose(true);
    setVolume(75);
    setThemeIndex(0);
    setLanguageIndex(0);
}
