project (ktpaccountskcm-plugin-kaccounts)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${ACCOUNTSQT_INCLUDE_DIRS}
                     ${SIGNONQT_INCLUDE_DIRS}

)

set (ktpaccountskcm_plugin_kaccounts_SRCS
     kaccounts-ui-provider.cpp
)

add_library (ktpaccountskcm_plugin_kaccounts MODULE
             ${ktpaccountskcm_plugin_kaccounts_SRCS}
)

target_link_libraries (ktpaccountskcm_plugin_kaccounts
                       ktpaccountskcminternal
                       Qt5::Core
                       Qt5::Widgets
                       ${TELEPATHY_QT5_LIBRARIES}
                       KAccounts
                       ${ACCOUNTSQT_LIBRARIES}
                       ${SIGNONQT_LIBRARIES}
)

# Install:
install (TARGETS ktpaccountskcm_plugin_kaccounts
         DESTINATION ${PLUGIN_INSTALL_DIR}/kaccounts/ui
)
