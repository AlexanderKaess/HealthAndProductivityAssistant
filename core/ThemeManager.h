#pragma once

#include <QObject>
#include <QString>

class ThemeManager : public QObject
{
    Q_OBJECT
public:
    enum Theme {
        LIGHT,
        DARK,
        SYSTEM
    };
    Q_ENUM(Theme)

    static ThemeManager& instance();
    void applyTheme(Theme theme);
    Theme getCcurrentTheme() const { return currentTheme; }

signals:
    void themeChanged(ThemeManager::Theme theme);

private:
    explicit ThemeManager(QObject *parent = nullptr);
    Theme resolveTheme(Theme theme) const;
    QString lightStyleSheet() const;
    QString darkStyleSheet() const;
    Theme currentTheme = SYSTEM;
};
