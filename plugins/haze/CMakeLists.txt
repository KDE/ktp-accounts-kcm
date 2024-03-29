project (ktpaccountskcm-plugin-haze)

add_definitions(-DTRANSLATION_DOMAIN=\"kcmtelepathyaccounts_plugin_haze\")

set (ktpaccountskcm_plugin_haze_SRCS
     haze-account-ui-plugin.cpp
     haze-icq-account.cpp
     icq-main-options-widget.cpp
     icq-server-settings-widget.cpp
     haze-myspaceim-account-ui.cpp
     myspaceim-main-options-widget.cpp
     myspaceim-advanced-settings-widget.cpp
     haze-skype-account-ui.cpp
     skype-main-options-widget.cpp
     skype-advanced-settings-widget.cpp
     haze-yahoo-account.cpp
     yahoo-main-options-widget.cpp
     yahoo-server-settings-widget.cpp
     aim-main-options-widget.cpp
     haze-aim-account.cpp
     aim-server-settings-widget.cpp
     msn-main-options-widget.cpp
     haze-msn-account.cpp
     haze-groupwise-account.cpp
     groupwise-main-options-widget.cpp
     groupwise-advanced-settings-widget.cpp
     haze-steam-account.cpp
     steam-main-options-widget.cpp
     haze-sipe-account.cpp
     sipe-main-options-widget.cpp
     sipe-advanced-settings-widget.cpp
     skypeweb-options-widget.cpp
     haze-skypeweb-account.cpp
)

ki18n_wrap_ui (ktpaccountskcm_plugin_haze_SRCS
                   icq-main-options-widget.ui
                   icq-server-settings-widget.ui
                   myspaceim-main-options-widget.ui
                   myspaceim-advanced-settings-widget.ui
                   skype-main-options-widget.ui
                   skype-advanced-settings-widget.ui
                   yahoo-main-options-widget.ui
                   yahoo-server-settings-widget.ui
                   aim-main-options-widget.ui
                   aim-server-settings-widget.ui
                   msn-main-options-widget.ui
                   groupwise-advanced-settings-widget.ui
                   groupwise-main-options-widget.ui
                   steam-main-options-widget.ui
                   sipe-main-options-widget.ui
                   sipe-advanced-settings-widget.ui
                   skypeweb-options-widget.ui
)

add_library (ktpaccountskcm_plugin_haze MODULE
                 ${ktpaccountskcm_plugin_haze_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_haze
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       KF5::Codecs
                       ${TELEPATHY_QT5_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_haze
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_haze.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_haze.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_haze.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
