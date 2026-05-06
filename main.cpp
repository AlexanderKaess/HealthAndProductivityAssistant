#include <QApplication>
#include <QDir>
#include "MainWindow.h"
#include <log4cxx/xml/domconfigurator.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // add app icon
    QFileInfo info(__FILE__);
    QString parentPath = info.path();
    a.setWindowIcon(QIcon(parentPath + "/ui/AppIcon_small.png"));
    a.setOrganizationName("MyCompany");
    a.setApplicationName("HealthProductivityApp");
    a.setQuitOnLastWindowClosed(false);

    // load configuration
    QString configPath = QCoreApplication::applicationDirPath() + "/log4cxx.xml";
    log4cxx::xml::DOMConfigurator::configure(configPath.toStdString());

    // create logger
    log4cxx::LoggerPtr logger{log4cxx::Logger::getLogger("HealthLogger.main")};
    LOG4CXX_INFO(logger, "----------");
    LOG4CXX_INFO(logger, "Program HealthAndProductivityAssistant started...");
    LOG4CXX_INFO(logger, "Working dir:" << QDir::currentPath().toStdString());

    MainWindow w;
    w.show();
    return a.exec();
}
