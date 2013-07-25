project (ktp-accounts-kcm)

set(KTP_ACCOUNTS_KCM_VERSION "0.6.2")

# Add the modules we ship to the module path
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

cmake_policy(SET CMP0002 OLD)

set(IS_KTP_INTERNAL_MODULE TRUE)
set(KDE_MIN_VERSION "4.7.0")
find_package (KDE4 REQUIRED)
find_package (TelepathyQt4 0.8.9 REQUIRED)
find_package (TelepathyLoggerQt4 QUIET)
find_package (KTp REQUIRED)

macro_log_feature(TELEPATHY_LOGGER_QT4_FOUND "TelepatyLoggerQt4" "Qt bindings for TelepathyLogger. This is needed to provide access to chat logs. HIGHLY recommended" "http://projects.kde.org/telepathy-logger-qt" FALSE "" "")
macro_display_feature_log()

# set some default settings
include (KDE4Defaults)

# make some more macros available
include (MacroLibrary)

add_definitions (${KDE4_DEFINITIONS}
                 -DQT_NO_CAST_FROM_ASCII
                 -DQT_NO_KEYWORDS
)

include_directories (${KDE4_INCLUDES}
                     ${TELEPATHY_QT4_INCLUDE_DIR}
                     ${KTP_INCLUDE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (TELEPATHY_LOGGER_QT4_FOUND)
    add_definitions(${TELEPATHY_LOGGER_QT4_DEFINITIONS} -DHAVE_TPLOGGERQT)
    include_directories(${TELEPATHY_LOGGER_QT4_INCLUDE_DIRS})
endif (TELEPATHY_LOGGER_QT4_FOUND)

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (plugins)
