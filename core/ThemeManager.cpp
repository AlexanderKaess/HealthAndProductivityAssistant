#include "Thememanager.h"
#include <QApplication>
#include <QStyleHints>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager mgr;
    return mgr;
}

ThemeManager::ThemeManager(QObject *parent) : QObject(parent) {
    connect(QApplication::styleHints(), &QStyleHints::colorSchemeChanged, this,
        [this](Qt::ColorScheme) {
            if (currentTheme == SYSTEM) {
                applyTheme(SYSTEM);
            }});
}

ThemeManager::Theme ThemeManager::resolveTheme(Theme theme) const {
    if (theme != SYSTEM) {
        return theme;
    }

    const auto scheme = QApplication::styleHints()->colorScheme();
    return (scheme == Qt::ColorScheme::Dark) ? DARK : LIGHT;
}

void ThemeManager::applyTheme(Theme theme)
{
    currentTheme = theme;
    const Theme effective = resolveTheme(theme);
    qApp->setStyleSheet(
        effective == DARK ? darkStyleSheet() : lightStyleSheet()
        );
    emit themeChanged(theme);
}

QString ThemeManager::lightStyleSheet() const
{
    return R"(
        QMainWindow, QDialog { background-color: #b0b0b0; color: #2c3e50; }
    )";
}

QString ThemeManager::darkStyleSheet() const
{
    return R"(
        QMainWindow, QDialog { background-color: #000000; color: #ecf0f1; }
    )";
}
