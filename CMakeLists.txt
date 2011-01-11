project (telepathy-accounts-kcm-plugins)

# Add the modules we ship to the module path
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package (KDE4 REQUIRED)
find_package (TelepathyQt4 REQUIRED)
find_package (KCMTelepathyAccounts REQUIRED)

# set some default settings
include (KDE4Defaults)

# make some more macros available
include (MacroLibrary)

add_definitions (${KDE4_DEFINITIONS})

include_directories (${KDE4_INCLUDES}
                     ${TELEPATHY_QT4_INCLUDE_DIR}
                     ${KCMTELEPATHYACCOUNTS_INCLUDE_DIR}
)

add_subdirectory (gabble)
add_subdirectory (butterfly)
add_subdirectory (idle)
add_subdirectory (haze)

