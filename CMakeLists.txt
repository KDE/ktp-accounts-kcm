project (telepathy-accounts-kcm)

# Add the modules we ship to the module path
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

find_package (KDE4 REQUIRED)
find_package (TelepathyQt4 REQUIRED)

# set some default settings
include (KDE4Defaults)

# make some more macros available
include (MacroLibrary)

add_definitions (${KDE4_DEFINITIONS})

include_directories (${KDE4_INCLUDES}
                     ${TELEPATHY_QT4_INCLUDE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_subdirectory (src)
add_subdirectory (data)
add_subdirectory (plugins)
