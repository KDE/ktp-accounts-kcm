project (ktp-accounts-kcm)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 5.2.0 CONFIG REQUIRED Core Widgets DBus)
find_package(KF5 5.11 REQUIRED KCMUtils KIO I18n WidgetsAddons Codecs CoreAddons ConfigWidgets IconThemes ItemViews)

# KDE Application Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "20")
set (RELEASE_SERVICE_VERSION_MINOR "04")
set (RELEASE_SERVICE_VERSION_MICRO "2")

set(KTP_ACCOUNTS_KCM_SONUMBER "9")
set(KTP_ACCOUNTS_KCM_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

set(IS_KTP_INTERNAL_MODULE TRUE)

find_package (TelepathyQt5 REQUIRED)
find_package (KAccounts REQUIRED)

set(CMAKE_MODULE_PATH ${KACCOUNTS_MACROS_PATH} ${CMAKE_MODULE_PATH}) #for KAccountsMacros

# make some more macros available
# include (MacroLibrary)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(FeatureSummary)
include(KAccountsMacros)

add_definitions (
#                  -DQT_NO_CAST_FROM_ASCII
                 -DQT_NO_KEYWORDS
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${TELEPATHY_QT5_INCLUDE_DIR})

option(BUILD_DISABLED_PROVIDERS "Build providers that are disabled by default" OFF)

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (plugins)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
