project (kcmtelepathyaccounts-plugin-gabble)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (kcmtelepathyaccounts_plugin_gabble_SRCS
     gabble-account-ui-plugin.cpp
     gabble-account-ui.cpp
     mandatory-parameters-widget.cpp
)

kde4_add_ui_files (kcmtelepathyaccounts_plugin_gabble_SRCS
)

kde4_add_plugin (kcmtelepathyaccounts_plugin_gabble
                 ${kcmtelepathyaccounts_plugin_gabble_SRCS}
)

target_link_libraries (kcmtelepathyaccounts_plugin_gabble
                       ${KCMTELEPATHYACCOUNTS_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS kcmtelepathyaccounts_plugin_gabble
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcmtelepathyaccounts_plugin_gabble.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

