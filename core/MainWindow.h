#pragma once

#include <QMainWindow>
#include <log4cxx/logger.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onPomodoroTimerClicked();
    void onStayHydratedClicked();
    void onFreshAirTimerClicked();
    void onWorkingHourTimerClicked();
    void onBreakTimerClicked();
    void onMovementTimerClicked();

private:
    Ui::MainWindow *ui;
    log4cxx::LoggerPtr logger;
};
