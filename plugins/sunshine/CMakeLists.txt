project (ktpaccountskcm-plugin-sunshine)

add_definitions(-DTRANSLATION_DOMAIN=\"ktpaccountskcm_plugin_sunshine\")

set (ktpaccountskcm_plugin_sunshine_SRCS
     sunshine-account-ui-plugin.cpp
     sunshine-account-ui.cpp
     sunshine-main-options-widget.cpp
     sunshine-advanced-options-widget.cpp
)

ki18n_wrap_ui (ktpaccountskcm_plugin_sunshine_SRCS
                   sunshine-main-options-widget.ui
                   sunshine-advanced-options-widget.ui
)

add_library (ktpaccountskcm_plugin_sunshine MODULE
                 ${ktpaccountskcm_plugin_sunshine_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_sunshine
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_sunshine
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_sunshine.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sunshine.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sunshine.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
