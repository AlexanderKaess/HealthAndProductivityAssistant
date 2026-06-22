#include "ThemeManager.h"
#include <QApplication>
#include <QStyleHints>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager instance;
    return instance;
}

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
    , logger(log4cxx::Logger::getLogger("HealthLogger.ThemeManager"))
{
    LOG4CXX_INFO(logger, "ThemeManager started ...");

    connect(QApplication::styleHints(), &QStyleHints::colorSchemeChanged, this,
        [this](Qt::ColorScheme) {
            if (currentTheme == SYSTEM) {
                applyTheme(DARK);
            }});
}

IThemeManager::Theme ThemeManager::resolveTheme(IThemeManager::Theme theme) const {
    if (theme != SYSTEM) {
        return theme;
    }

    const auto scheme = QApplication::styleHints()->colorScheme();
    return (scheme == Qt::ColorScheme::Dark) ? DARK : LIGHT;
}

void ThemeManager::applyTheme(IThemeManager::Theme theme)
{
    LOG4CXX_INFO(logger, "Change Theme");
    currentTheme = theme;
    const IThemeManager::Theme effective = resolveTheme(theme);
    qApp->setStyleSheet(
        effective == DARK ? darkStyleSheet() : lightStyleSheet()
        );
    emit themeChanged(theme);
}

QString ThemeManager::lightStyleSheet() const
{
    LOG4CXX_INFO(logger, "Theme changed to LIGHT");
    return R"(
        QMainWindow, QDialog { background-color: #b0b0b0; color: #2c3e50; }
    )";
}

QString ThemeManager::darkStyleSheet() const
{
    LOG4CXX_INFO(logger, "Theme changed to DARK");
    return R"(
        QMainWindow, QDialog { background-color: #353535; color: #ecf0f1; }
    )";
}
