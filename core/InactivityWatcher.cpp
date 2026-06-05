#include "InactivityWatcher.h"

InactivityWatcher::InactivityWatcher(QObject *parent)
    : QObject(parent)
    , logger(log4cxx::Logger::getLogger("HealthLogger.InactivityWatcher"))
{
    timer.setSingleShot(true);
    LOG4CXX_INFO(logger, "InactivityWatcher created");

    connect(&timer, &QTimer::timeout, this, &InactivityWatcher::inactivityDetected);
}

InactivityWatcher::~InactivityWatcher() {
    stop();
}

void InactivityWatcher::start(int timeOutMs) {
    if (!QCoreApplication::instance()) {
        qWarning("InactivityWatcher::start() — QApplication not initilized");
        return;
    }

    if (running){
        stop();
    }

    LOG4CXX_INFO(logger, "InactivityWatcher started ...");
    QCoreApplication::instance()->installEventFilter(this);
    running = true;
    timer.start(timeOutMs);
}

void InactivityWatcher::stop() {
    LOG4CXX_INFO(logger, "InactivityWatcher stopped ...");
    if (!running){
        return;
    }
    timer.stop();
    if (QCoreApplication::instance()){
        QCoreApplication::instance()->removeEventFilter(this);
    }
    running = false;
}

bool InactivityWatcher::eventFilter(QObject *watched, QEvent *event) {
    const QEvent::Type type = event->type();
    const bool isKeyboard = (type == QEvent::KeyPress || type == QEvent::KeyRelease);

    const bool isMouse = (
        type == QEvent::MouseMove || type == QEvent::MouseButtonPress || type == QEvent::MouseButtonRelease
    );

    if (isKeyboard || isMouse) {
        resetTimer();
    }

    return QObject::eventFilter(watched, event);
}

void InactivityWatcher::resetTimer() {
    if (running) {
        // use the same timeout
        timer.start();
    }
}
