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

ki18n_wrap_ui (ktpaccountskcm_plugin_salut_SRCS
                   salut-main-options-widget.ui
                   salut-advanced-options-widget.ui
)

add_library  (ktpaccountskcm_plugin_salut MODULE
                 ${ktpaccountskcm_plugin_salut_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_salut
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       ${TELEPATHY_QT5LIBRARIES}
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
