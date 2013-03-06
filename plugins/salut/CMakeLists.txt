project (ktpaccountskcm-plugin-salut)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_salut_SRCS
     salut-account-ui-plugin.cpp
     salut-account-ui.cpp
     salut-main-options-widget.cpp
     salut-advanced-options-widget.cpp
)

kde4_add_ui_files (ktpaccountskcm_plugin_salut_SRCS
                   salut-main-options-widget.ui
                   salut-advanced-options-widget.ui
)

kde4_add_plugin (ktpaccountskcm_plugin_salut
                 ${ktpaccountskcm_plugin_salut_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_salut
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_salut
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_salut.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_salut.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_salut.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
