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
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES ktpaccountskcm_plugin_butterfly.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

