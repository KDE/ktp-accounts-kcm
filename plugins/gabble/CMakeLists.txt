project (ktpaccountskcm-plugin-gabble)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_gabble_SRCS
     gabble-account-ui-plugin.cpp
     gabble-account-ui.cpp
     main-options-widget.cpp
     main-options-widget-googletalk.cpp
     main-options-widget-facebook.cpp
     main-options-widget-msn.cpp
     main-options-widget-kde-talk.cpp
     server-settings-widget.cpp
     proxy-settings-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_gabble_SRCS
                   main-options-widget.ui
                   main-options-widget-googletalk.ui
                   main-options-widget-facebook.ui
                   main-options-widget-msn.ui
                   main-options-widget-kde-talk.ui
                   server-settings-widget.ui
                   proxy-settings-widget.ui
)


kde4_add_plugin (ktpaccountskcm_plugin_gabble
                 ${ktpaccountskcm_plugin_gabble_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_gabble
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_gabble
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_gabble.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_gabble.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_gabble.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
