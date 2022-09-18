project (ktpaccountskcm-plugin-morse)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

add_definitions(-DTRANSLATION_DOMAIN=\"kcmtelepathyaccounts_plugin_morse\")

set (ktpaccountskcm_plugin_morse_SRCS
     morse-account-ui-plugin.cpp
     telegram-account-ui.cpp
     main-options-widget.cpp
)

ki18n_wrap_ui (ktpaccountskcm_plugin_morse_SRCS
                   main-options-widget.ui
)

add_library (ktpaccountskcm_plugin_morse MODULE
                 ${ktpaccountskcm_plugin_morse_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_morse
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       ${TELEPATHY_QT5_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_morse
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_morse.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_morse.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_morse.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
