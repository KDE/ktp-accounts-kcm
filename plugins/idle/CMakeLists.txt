project (ktpaccountskcm-plugin-idle)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_idle_SRCS
     idle-account-ui-plugin.cpp
     idle-account-ui.cpp
     main-options-widget.cpp
     advanced-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_idle_SRCS
                   main-options-widget.ui
                   advanced-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_idle
                 ${ktpaccountskcm_plugin_idle_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_idle
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_idle
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES ktpaccountskcm_plugin_idle.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
