#include "InactivityWatcher.h"

InactivityWatcher::InactivityWatcher(QObject *parent)
    : QObject(parent)
    , logger(log4cxx::Logger::getLogger("HealthLogger.InactivityWatcher"))
{
    LOG4CXX_INFO(logger, "InactivityWatcher created");
    timer.setSingleShot(true);

    connect(&timer, &QTimer::timeout, this, &InactivityWatcher::inactivityDetected);
}

InactivityWatcher::~InactivityWatcher() {
    stop();
}

void InactivityWatcher::start(int timeOutMs) {
    LOG4CXX_INFO(logger, "InactivityWatcher started ...");
    qApp->installEventFilter(this);
    running = true;
    timer.start(timeOutMs);
}

void InactivityWatcher::stop() {
    LOG4CXX_INFO(logger, "InactivityWatcher stopped ...");
    timer.stop();
    qApp->removeEventFilter(this);
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
