project (ktpaccountskcm-plugin-butterfly)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_butterfly_SRCS
     butterfly-account-ui-plugin.cpp
     butterfly-account-ui.cpp
     main-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_butterfly_SRCS
                   main-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_butterfly
                 ${ktpaccountskcm_plugin_butterfly_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_butterfly
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_butterfly
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_butterfly.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_butterfly.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_butterfly.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
