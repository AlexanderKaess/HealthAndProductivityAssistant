#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "SoundManager.h"

class SoundManagerTest : public ::testing::Test
{
protected:
    void SetUp() override {
        soundManager = &SoundManager::instance();
    }

    SoundManager* soundManager { nullptr };
};

TEST_F(SoundManagerTest, InstanceIsNotNull)
{
    EXPECT_NE(soundManager, nullptr);
}

TEST_F(SoundManagerTest, SingletonReturnsSameInstance)
{
    EXPECT_EQ(soundManager, &SoundManager::instance());
}

TEST_F(SoundManagerTest, DefaultVolumeIs75)
{
    EXPECT_EQ(soundManager->getVolume(), 75);
}

TEST_F(SoundManagerTest, SetVolumeUpdates)
{
    soundManager->setVolume(50);
    EXPECT_EQ(soundManager->getVolume(), 50);
    soundManager->setVolume(75); // restore
}

TEST_F(SoundManagerTest, SetVolumeClampsToRange)
{
    soundManager->setVolume(200);
    EXPECT_LE(soundManager->getVolume(), 100);

    soundManager->setVolume(-5);
    EXPECT_GE(soundManager->getVolume(), 0);
}
