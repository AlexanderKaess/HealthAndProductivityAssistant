#pragma once

#include <QObject>
#include <QString>
#include <log4cxx/logger.h>

#include "IThemeManager.h"

class ThemeManager : public QObject, public IThemeManager
{
    Q_OBJECT
public:
    static ThemeManager& instance();
    void applyTheme(IThemeManager::Theme theme) override;
    IThemeManager::Theme getCurrentTheme() const { return currentTheme; }

signals:
    void themeChanged(IThemeManager::Theme theme);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    IThemeManager::Theme resolveTheme(IThemeManager::Theme theme) const;
    QString lightStyleSheet() const;
    QString darkStyleSheet() const;

    log4cxx::LoggerPtr logger;
    IThemeManager::Theme currentTheme = IThemeManager::Theme::SYSTEM;
};
