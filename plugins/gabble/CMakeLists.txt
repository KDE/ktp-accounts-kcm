project (ktpaccountskcm-plugin-gabble)

add_definitions(-DTRANSLATION_DOMAIN=\"kcmtelepathyaccounts_plugin_gabble\")

set (ktpaccountskcm_plugin_gabble_SRCS
     gabble-account-ui-plugin.cpp
     gabble-account-ui.cpp
     main-options-widget.cpp
     main-options-widget-googletalk.cpp
     main-options-widget-facebook.cpp
     main-options-widget-msn.cpp
     main-options-widget-kde-talk.cpp
     server-settings-widget.cpp
     proxy-settings-widget.cpp
)

ki18n_wrap_ui (ktpaccountskcm_plugin_gabble_SRCS
                   main-options-widget.ui
                   main-options-widget-googletalk.ui
                   main-options-widget-facebook.ui
                   main-options-widget-msn.ui
                   main-options-widget-kde-talk.ui
                   server-settings-widget.ui
                   proxy-settings-widget.ui
)


add_library (ktpaccountskcm_plugin_gabble MODULE
             ${ktpaccountskcm_plugin_gabble_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_gabble
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       ${TELEPATHY_QT5_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_gabble
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_gabble.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_gabble.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_gabble.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
