project (ktpaccountskcm-plugin-sipe)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_sipe_SRCS
     sipe-account-ui-plugin.cpp
     sipe-account-ui.cpp
     sipe-main-options-widget.cpp
     sipe-advanced-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_sipe_SRCS
                   sipe-main-options-widget.ui
                   sipe-advanced-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_sipe
                 ${ktpaccountskcm_plugin_sipe_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_sipe
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_sipe
         DESTINATION ${PLUGIN_INSTALL_DIR}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_sipe.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sipe.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sipe.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
