#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <QApplication>

#include "ThemeManager.h"

class ThemeManagerTest : public ::testing::Test
{
protected:
    void SetUp() override {
        themeManager = &ThemeManager::instance();
    }
    ThemeManager* themeManager { nullptr };
};

TEST_F(ThemeManagerTest, InstanceIsNotNull)
{
    EXPECT_NE(themeManager, nullptr);
}

TEST_F(ThemeManagerTest, SingletonReturnsSameInstance)
{
    EXPECT_EQ(themeManager, &ThemeManager::instance());
}

TEST_F(ThemeManagerTest, DefaultThemeIsSystem)
{
    EXPECT_EQ(themeManager->getCurrentTheme(), ThemeManager::SYSTEM);
}

TEST_F(ThemeManagerTest, ApplyThemeDarkDoesNotCrash)
{
    EXPECT_NO_THROW(themeManager->applyTheme(ThemeManager::DARK));
}

TEST_F(ThemeManagerTest, ApplyThemeLightDoesNotCrash)
{
    EXPECT_NO_THROW(themeManager->applyTheme(ThemeManager::LIGHT));
}

TEST_F(ThemeManagerTest, ApplyThemeUpdatesIndex)
{
    themeManager->applyTheme(ThemeManager::DARK);
    EXPECT_EQ(themeManager->getCurrentTheme(), ThemeManager::DARK);
    themeManager->applyTheme(ThemeManager::SYSTEM);
}
