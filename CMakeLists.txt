project (ktp-accounts-kcm)

cmake_minimum_required(VERSION 2.8.12)

find_package(ECM 1.0.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

find_package(Qt5 5.2.0 CONFIG REQUIRED Core Widgets DBus)
find_package(KF5 5.0.0 REQUIRED KCMUtils KIO I18n WidgetsAddons Codecs CoreAddons ConfigWidgets IconThemes ItemViews)

set(KTP_ACCOUNTS_KCM_SONUMBER "9")
set(KTP_ACCOUNTS_KCM_VERSION "0.${KTP_ACCOUNTS_KCM_SONUMBER}.99")

set(IS_KTP_INTERNAL_MODULE TRUE)

find_package (TelepathyQt5 REQUIRED)
find_package (KAccounts REQUIRED)
find_package (AccountsFileDir REQUIRED)
find_package (Intltool REQUIRED) #needed to generate service and provider files

# make some more macros available
# include (MacroLibrary)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(FeatureSummary)

add_definitions (
#                  -DQT_NO_CAST_FROM_ASCII
                 -DQT_NO_KEYWORDS
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${TELEPATHY_QT5_INCLUDE_DIR})

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (plugins)
