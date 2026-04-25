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
    connect(ui->pomodoroPushButton, &QPushButton::clicked, this, &MainWindow::onPomodoroClicked);

    statusBar()->showMessage("ready");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPomodoroClicked()
{
    TimerDialog dialog(TimerDialog::Pomodoro, this);
    dialog.exec();
}
