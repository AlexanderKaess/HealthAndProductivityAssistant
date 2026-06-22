#pragma once

#include <QObject>
class IThemeManager
{
public:
    enum Theme {
        LIGHT,
        DARK,
        SYSTEM
    };

    virtual ~IThemeManager() = default;
    virtual void applyTheme(Theme theme) = 0;
};
