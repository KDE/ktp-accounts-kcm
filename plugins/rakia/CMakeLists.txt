project (ktpaccountskcm-plugin-rakia)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_rakia_SRCS
     rakia-account-ui-plugin.cpp
     rakia-account-ui.cpp
     rakia-main-options-widget.cpp
     rakia-advanced-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_rakia_SRCS
                   rakia-main-options-widget.ui
                   rakia-advanced-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_rakia
                 ${ktpaccountskcm_plugin_rakia_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_rakia
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_rakia
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_rakia.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_rakia.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_rakia.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
