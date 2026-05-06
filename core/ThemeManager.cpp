#include "thememanager.h"
#include <QApplication>
#include <QStyleHints>

ThemeManager& ThemeManager::instance()
{
    static ThemeManager mgr;
    return mgr;
}

ThemeManager::ThemeManager(QObject *parent) : QObject(parent) {}

void ThemeManager::applyTheme(Theme theme)
{
    currentTheme = theme;
    Theme effective = theme;

    if (theme == System) {
        auto scheme = QApplication::styleHints()->colorScheme();
        effective = (scheme == Qt::ColorScheme::Dark) ? Dark : Light;
    }

    qApp->setStyleSheet(effective == Dark ? darkStyleSheet() : lightStyleSheet());
    emit themeChanged(theme);
}

QString ThemeManager::lightStyleSheet() const
{
    return R"(
        QMainWindow, QDialog { background-color: #f5f5f5; color: #2c3e50; }
        QGroupBox {
            border: 1px solid #bdc3c7; border-radius: 5px;
            margin-top: 10px; padding-top: 10px; font-weight: bold;
        }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }
        QTableWidget { background-color: white; alternate-background-color: #ecf0f1; }
        QTabWidget::pane { border: 1px solid #bdc3c7; background: white; }
    )";
}

QString ThemeManager::darkStyleSheet() const
{
    return R"(
        QMainWindow, QDialog { background-color: #2c3e50; color: #ecf0f1; }
        QLabel { color: #ecf0f1; }
        QGroupBox {
            border: 1px solid #34495e; border-radius: 5px;
            margin-top: 10px; padding-top: 10px;
            font-weight: bold; color: #ecf0f1;
        }
        QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }
        QTableWidget {
            background-color: #34495e; alternate-background-color: #3d566e;
            color: #ecf0f1; gridline-color: #2c3e50;
        }
        QHeaderView::section { background-color: #1a252f; color: #ecf0f1; padding: 5px; }
        QTabWidget::pane { border: 1px solid #34495e; background: #34495e; }
        QTabBar::tab { background: #34495e; color: #ecf0f1; padding: 10px 20px; }
        QTabBar::tab:selected { background: #3498db; color: white; }
        QLineEdit, QSpinBox, QComboBox {
            background: #34495e; color: #ecf0f1; border: 1px solid #1a252f;
            border-radius: 3px; padding: 4px;
        }
        QCheckBox { color: #ecf0f1; }
        QStatusBar { background: #1a252f; color: #ecf0f1; }
    )";
}
