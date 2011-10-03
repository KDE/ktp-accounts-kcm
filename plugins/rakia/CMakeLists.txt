project (kcmtelepathyaccounts-plugin-rakia)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (kcmtelepathyaccounts_plugin_rakia_SRCS
     rakia-account-ui-plugin.cpp
     rakia-account-ui.cpp
     rakia-main-options-widget.cpp
     rakia-advanced-options-widget.cpp
)

kde4_add_ui_files (kcmtelepathyaccounts_plugin_rakia_SRCS
                   rakia-main-options-widget.ui
                   rakia-advanced-options-widget.ui
)

kde4_add_plugin (kcmtelepathyaccounts_plugin_rakia
                 ${kcmtelepathyaccounts_plugin_rakia_SRCS}
)

target_link_libraries (kcmtelepathyaccounts_plugin_rakia
                       ${KCMTELEPATHYACCOUNTS_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS kcmtelepathyaccounts_plugin_rakia
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcmtelepathyaccounts_plugin_rakia.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

