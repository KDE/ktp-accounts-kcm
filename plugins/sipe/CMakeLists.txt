project (ktpaccountskcm-plugin-sipe)

add_definitions(-DTRANSLATION_DOMAIN=\"kcmtelepathyaccounts_plugin_sipe\")

set (ktpaccountskcm_plugin_sipe_SRCS
     sipe-account-ui-plugin.cpp
     sipe-account-ui.cpp
     sipe-main-options-widget.cpp
     sipe-advanced-options-widget.cpp
)

ki18n_wrap_ui (ktpaccountskcm_plugin_sipe_SRCS
             sipe-main-options-widget.ui
             sipe-advanced-options-widget.ui
)

add_library (ktpaccountskcm_plugin_sipe MODULE
             ${ktpaccountskcm_plugin_sipe_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_sipe
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       ${TELEPATHY_QT5_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_sipe
         DESTINATION ${KDE_INSTALL_PLUGINDIR}
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_sipe.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sipe.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_sipe.desktop
         DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
)
