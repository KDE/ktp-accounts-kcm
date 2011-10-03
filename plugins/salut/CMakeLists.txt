project (kcmtelepathyaccounts-plugin-salut)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (kcmtelepathyaccounts_plugin_salut_SRCS
     salut-account-ui-plugin.cpp
     salut-account-ui.cpp
     salut-main-options-widget.cpp
     salut-advanced-options-widget.cpp
)

kde4_add_ui_files (kcmtelepathyaccounts_plugin_salut_SRCS
                   salut-main-options-widget.ui
                   salut-advanced-options-widget.ui
)

kde4_add_plugin (kcmtelepathyaccounts_plugin_salut
                 ${kcmtelepathyaccounts_plugin_salut_SRCS}
)

target_link_libraries (kcmtelepathyaccounts_plugin_salut
                       kcmtelepathyaccounts
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS kcmtelepathyaccounts_plugin_salut
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcmtelepathyaccounts_plugin_salut.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

