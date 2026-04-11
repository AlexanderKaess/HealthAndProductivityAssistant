#include "MainWindow.h"
#include "../ui/ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , logger(log4cxx::Logger::getLogger("HealthLogger.MainWindow"))

{
    ui->setupUi(this);
    setWindowTitle("HealthAndProductivityAssistant");
    LOG4CXX_INFO(logger, "Mainwindow started...");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addTimerPushButton_clicked()
{
    LOG4CXX_INFO(logger, "add timer button clicked");
}

void MainWindow::on_removeTimerPushButton_clicked()
{
    LOG4CXX_INFO(logger, "remove timer button clicked");
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    LOG4CXX_INFO(logger, "Index of tab: " << ui->tabWidget->currentIndex());
    LOG4CXX_INFO(logger,
                 "Index of tab: " << ui->tabWidget->tabText(ui->tabWidget->currentIndex())
                                         .toStdString());
}
