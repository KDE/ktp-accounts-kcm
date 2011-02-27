project (kcmtelepathyaccounts-plugin-sunshine)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (kcmtelepathyaccounts_plugin_sunshine_SRCS
     sunshine-account-ui-plugin.cpp
     sunshine-account-ui.cpp
     sunshine-main-options-widget.cpp
     sunshine-advanced-options-widget.cpp
)

kde4_add_ui_files (kcmtelepathyaccounts_plugin_sunshine_SRCS
                   sunshine-main-options-widget.ui
                   sunshine-advanced-options-widget.ui
)

kde4_add_plugin (kcmtelepathyaccounts_plugin_sunshine
                 ${kcmtelepathyaccounts_plugin_sunshine_SRCS}
)

target_link_libraries (kcmtelepathyaccounts_plugin_sunshine
                       ${KCMTELEPATHYACCOUNTS_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS kcmtelepathyaccounts_plugin_sunshine
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcmtelepathyaccounts_plugin_sunshine.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

