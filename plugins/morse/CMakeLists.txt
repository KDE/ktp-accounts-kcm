project (ktpaccountskcm-plugin-morse)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_morse_SRCS
     morse-account-ui-plugin.cpp
     telegram-account-ui.cpp
     main-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_morse_SRCS
                   main-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_morse
                 ${ktpaccountskcm_plugin_morse_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_morse
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_morse
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_morse.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_morse.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_morse.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
