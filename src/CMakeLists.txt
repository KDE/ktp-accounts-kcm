include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory (KCMTelepathyAccounts)

project (telepathy-accounts-kcm)

set (telepathy_accounts_kcm_SRCS
     salut-enabler.cpp
     salut-message-widget.cpp
     kcm-telepathy-accounts.cpp
     accounts-list-model.cpp
     account-item.cpp
     add-account-assistant.cpp
     edit-account-dialog.cpp
     salut-details-dialog.cpp
     accounts-list-delegate.cpp
)

kde4_add_ui_files (telepathy_accounts_kcm_SRCS
                   main-widget.ui
)

kde4_add_plugin (kcm_telepathy_accounts
                 ${telepathy_accounts_kcm_SRCS}
)

target_link_libraries (kcm_telepathy_accounts
                       kcmtelepathyaccounts
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${KTELEPATHY_LIBRARIES}
)

# Install:
install (TARGETS kcm_telepathy_accounts
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcm_telepathy_accounts.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

install (FILES settings-im-and-voip.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
