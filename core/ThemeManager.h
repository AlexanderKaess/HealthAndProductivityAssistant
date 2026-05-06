#pragma once

#include <QObject>
#include <QString>

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    enum Theme {
        Light,
        Dark,
        System};
    Q_ENUM(Theme)

    static ThemeManager& instance();
    void applyTheme(Theme theme);
    Theme getCcurrentTheme() const { return currentTheme; }

signals:
    void themeChanged(ThemeManager::Theme theme);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    QString lightStyleSheet() const;
    QString darkStyleSheet() const;
    Theme currentTheme = Light;
};
