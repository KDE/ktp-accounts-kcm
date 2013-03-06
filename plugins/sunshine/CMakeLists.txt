project (ktpaccountskcm-plugin-sunshine)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_sunshine_SRCS
     sunshine-account-ui-plugin.cpp
     sunshine-account-ui.cpp
     sunshine-main-options-widget.cpp
     sunshine-advanced-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_sunshine_SRCS
                   sunshine-main-options-widget.ui
                   sunshine-advanced-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_sunshine
                 ${ktpaccountskcm_plugin_sunshine_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_sunshine
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_sunshine
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_sunshine.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sunshine.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sunshine.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
