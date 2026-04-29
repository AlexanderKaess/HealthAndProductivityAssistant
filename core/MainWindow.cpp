#include "MainWindow.h"
#include "../ui/ui_MainWindow.h"
#include "TimerDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logger(log4cxx::Logger::getLogger("HealthLogger.MainWindow"))

{
    ui->setupUi(this);
    setWindowTitle("HealthAndProductivityAssistant");
    LOG4CXX_INFO(logger, "Mainwindow started...");

    // Connect buttons to slots
    connect(ui->pomodoroPushButton,&QPushButton::clicked,this,&MainWindow::onPomodoroTimerClicked);
    connect(ui->stayHydratedPushButton,&QPushButton::clicked,this,&MainWindow::onStayHydratedClicked);
    connect(ui->freshAirPushButton,&QPushButton::clicked,this,&MainWindow::onFreshAirTimerClicked);
    connect(ui->workingHoursPushButton,&QPushButton::clicked,this,&MainWindow::onWorkingHourTimerClicked);
    connect(ui->breakPushButton, &QPushButton::clicked, this, &MainWindow::onBreakTimerClicked);
    connect(ui->movementPushButton,&QPushButton::clicked,this,&MainWindow::onMovementTimerClicked);

    statusBar()->showMessage("ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPomodoroTimerClicked()
{
    LOG4CXX_INFO(logger, "TimerDialog::POMODORO started...");
    TimerDialog dialog(TimerDialog::POMODORO, this);
    dialog.exec();
}

void MainWindow::onStayHydratedClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::STAYHYDRATET started...");
    TimerDialog dialog(TimerDialog::STAYHYDRATET, this);
    dialog.exec();
}

void MainWindow::onFreshAirTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::FRESHAIR started...");
    TimerDialog dialog(TimerDialog::FRESHAIR, this);
    dialog.exec();
}

void MainWindow::onWorkingHourTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::WORKINGHOUR started...");
    TimerDialog dialog(TimerDialog::WORKINGHOUR, this);
    dialog.exec();
}

void MainWindow::onBreakTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::BREAK started...");
    TimerDialog dialog(TimerDialog::BREAK, this);
    dialog.exec();
}

void MainWindow::onMovementTimerClicked() {
    LOG4CXX_INFO(logger, "TimerDialog::MOVEMENT started...");
    TimerDialog dialog(TimerDialog::MOVEMENT, this);
    dialog.exec();
}
