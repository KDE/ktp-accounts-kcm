cmake_minimum_required(VERSION 3.16)

# KDE Gear Version, managed by release script
set (RELEASE_SERVICE_VERSION_MAJOR "22")
set (RELEASE_SERVICE_VERSION_MINOR "08")
set (RELEASE_SERVICE_VERSION_MICRO "0")

set(KTP_ACCOUNTS_KCM_SONUMBER "9")
set(KTP_ACCOUNTS_KCM_VERSION "${RELEASE_SERVICE_VERSION_MAJOR}.${RELEASE_SERVICE_VERSION_MINOR}.${RELEASE_SERVICE_VERSION_MICRO}")

project (ktp-accounts-kcm VERSION ${KTP_ACCOUNTS_KCM_VERSION})

set(QT_MIN_VERSION "5.15.0")
set(KF_MIN_VERSION "5.85.0")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMInstallIcons)
include(FeatureSummary)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
    Core
    DBus
    Widgets
)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED
    Codecs
    ConfigWidgets
    CoreAddons
    I18n
    IconThemes
    ItemViews
    KCMUtils
    KIO
    WidgetsAddons
)

find_package (TelepathyQt5 REQUIRED)
find_package (KAccounts REQUIRED)

set(CMAKE_MODULE_PATH ${KACCOUNTS_MACROS_PATH} ${CMAKE_MODULE_PATH}) #for KAccountsMacros

# make some more macros available
# include (MacroLibrary)

include(KAccountsMacros)

# added by KDE_COMPILERSETTINGS_LEVEL 5.85
remove_definitions (
    -DQT_NO_FOREACH
)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${TELEPATHY_QT5_INCLUDE_DIR})

option(BUILD_DISABLED_PROVIDERS "Build providers that are disabled by default" OFF)

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (plugins)

ki18n_install(po)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
