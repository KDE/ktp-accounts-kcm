project (ktpaccountskcm-plugin-idle)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (ktpaccountskcm_plugin_idle_SRCS
     idle-account-ui-plugin.cpp
     idle-account-ui.cpp
     main-options-widget.cpp
     advanced-options-widget.cpp
)

ki18n_wrap_ui (ktpaccountskcm_plugin_idle_SRCS
                   main-options-widget.ui
                   advanced-options-widget.ui
)

add_library (ktpaccountskcm_plugin_idle MODULE
                 ${ktpaccountskcm_plugin_idle_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_idle
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       KF5::Codecs
                       ${TELEPATHY_QT5_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_idle
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_idle.desktop.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_idle.desktop
               @ONLY)

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_idle.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
