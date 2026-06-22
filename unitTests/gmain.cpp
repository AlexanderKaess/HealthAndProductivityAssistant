#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QApplication>
#include <log4cxx/basicconfigurator.h>

int main(int argc, char* argv[]) {
    //configure log4cxx, so there are no warning messages
    log4cxx::BasicConfigurator::configure();

    // qt needs a qapplication instance for widget tests
    QApplication app(argc, argv);

    // initialize gtest and gmock
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
