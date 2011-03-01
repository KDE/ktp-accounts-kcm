project (kcmtelepathyaccounts-plugin-haze)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (kcmtelepathyaccounts_plugin_haze_SRCS
     haze-account-ui-plugin.cpp
     haze-icq-account.cpp
     icq-main-options-widget.cpp
     icq-server-settings-widget.cpp
     haze-skype-account-ui.cpp
     skype-main-options-widget.cpp
     skype-advanced-settings-widget.cpp
     haze-yahoo-account.cpp
     yahoo-main-options-widget.cpp
     yahoo-server-settings-widget.cpp
)

kde4_add_ui_files (kcmtelepathyaccounts_plugin_haze_SRCS
                   icq-main-options-widget.ui
                   icq-server-settings-widget.ui
                   skype-main-options-widget.ui
                   skype-advanced-settings-widget.ui
                   yahoo-main-options-widget.ui
                   yahoo-server-settings-widget.ui
)

kde4_add_plugin (kcmtelepathyaccounts_plugin_haze
                 ${kcmtelepathyaccounts_plugin_haze_SRCS}
)

target_link_libraries (kcmtelepathyaccounts_plugin_haze
                       ${KCMTELEPATHYACCOUNTS_LIBRARIES}
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS kcmtelepathyaccounts_plugin_haze
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcmtelepathyaccounts_plugin_haze.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
