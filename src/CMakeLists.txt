include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory (KCMTelepathyAccounts)

project (ktp-accounts-kcm)

set (kcm_ktp_accounts_SRCS
     avatar-button.cpp
     fetch-avatar-job.cpp
     salut-enabler.cpp
     salut-message-widget.cpp
     kcm-telepathy-accounts.cpp
     accounts-list-model.cpp
     account-item.cpp
     add-account-assistant.cpp
     edit-account-dialog.cpp
     account-identity-dialog.cpp
     salut-details-dialog.cpp
     accounts-list-delegate.cpp
     edit-display-name-button.cpp
)


kde4_add_ui_files (kcm_ktp_accounts_SRCS
                   main-widget.ui
                   account-identity-dialog.ui
)

kde4_add_plugin (kcm_ktp_accounts
                 ${kcm_ktp_accounts_SRCS}
)

target_link_libraries (kcm_ktp_accounts
                       ktpaccountskcminternal
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
                       ${KTP_LIBRARIES}
                       ${KDE4_KIO_LIBS}

)

# Install:
install (TARGETS kcm_ktp_accounts
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcm_ktp_accounts.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)

install (FILES settings-im-and-voip.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
