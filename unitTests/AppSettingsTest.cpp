#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QCoreApplication>

#include "AppSettings.h"

class AppSettingsTest : public ::testing::Test
{
protected:
    void SetUp() override {
        QCoreApplication::setOrganizationName("TestOrg");
        QCoreApplication::setApplicationName("TestApp");
        settings = std::make_unique<AppSettings>();
    }

    void TearDown() override {
        QSettings s;
        s.clear();
    }

    std::unique_ptr<AppSettings> settings;
};

TEST_F(AppSettingsTest, DefaultSoundEnabled)
{
    EXPECT_TRUE(settings->getSoundEnabled());
}

TEST_F(AppSettingsTest, DefaultPopupEnabled)
{
    EXPECT_TRUE(settings->getPopupEnabled());
}

TEST_F(AppSettingsTest, DefaultConfirmClose)
{
    EXPECT_TRUE(settings->getConfirmClose());
}

TEST_F(AppSettingsTest, DefaultVolume)
{
    EXPECT_EQ(settings->getVolume(), 75);
}

TEST_F(AppSettingsTest, DefaultThemeIndex)
{
    EXPECT_EQ(settings->getThemeIndex(), 0);
}

TEST_F(AppSettingsTest, DefaultLanguageIndex)
{
    EXPECT_EQ(settings->getLanguageIndex(), 0);
}

TEST_F(AppSettingsTest, SetSoundEnabled)
{
    settings->setSoundEnabled(false);
    EXPECT_FALSE(settings->getSoundEnabled());
}

TEST_F(AppSettingsTest, SetVolumeClampsAbove100)
{
    settings->setVolume(200);
    EXPECT_EQ(settings->getVolume(), 100);
}

TEST_F(AppSettingsTest, SetVolumeClampsBelowZero)
{
    settings->setVolume(-10);
    EXPECT_EQ(settings->getVolume(), 0);
}

TEST_F(AppSettingsTest, SetVolumeValid)
{
    settings->setVolume(50);
    EXPECT_EQ(settings->getVolume(), 50);
}

TEST_F(AppSettingsTest, SetThemeIndex)
{
    settings->setThemeIndex(2);
    EXPECT_EQ(settings->getThemeIndex(), 2);
}

TEST_F(AppSettingsTest, SetLanguageIndex)
{
    settings->setLanguageIndex(1);
    EXPECT_EQ(settings->getLanguageIndex(), 1);
}

TEST_F(AppSettingsTest, SoundEnabledChangedSignalEmitted)
{
    bool received = false;
    QObject::connect(settings.get(), &AppSettings::soundEnabledChanged,
                     [&](bool value){ received = true; EXPECT_FALSE(value); });
    settings->setSoundEnabled(false);
    EXPECT_TRUE(received);
}

TEST_F(AppSettingsTest, NoSignalIfValueUnchanged)
{
    int count = 0;
    QObject::connect(settings.get(), &AppSettings::soundEnabledChanged,
                     [&](bool){ ++count; });
    settings->setSoundEnabled(true);
    EXPECT_EQ(count, 0);
}

TEST_F(AppSettingsTest, VolumeChangedSignalEmitted)
{
    int received = -1;
    QObject::connect(settings.get(), &AppSettings::volumeChanged,
                     [&](int volume){ received = volume; });
    settings->setVolume(42);
    EXPECT_EQ(received, 42);
}

TEST_F(AppSettingsTest, SaveAndLoad)
{
    settings->setSoundEnabled(false);
    settings->setVolume(33);
    settings->setThemeIndex(1);
    settings->save();

    AppSettings loadedSettings;
    loadedSettings.load();
    EXPECT_FALSE(loadedSettings.getSoundEnabled());
    EXPECT_EQ(loadedSettings.getVolume(), 33);
    EXPECT_EQ(loadedSettings.getThemeIndex(), 1);
}

TEST_F(AppSettingsTest, ResetRestoresDefaults)
{
    settings->setVolume(10);
    settings->setSoundEnabled(false);
    settings->reset();

    EXPECT_EQ(settings->getVolume(), 75);
    EXPECT_TRUE(settings->getSoundEnabled());
}
