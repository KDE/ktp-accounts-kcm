add_subdirectory (libkcmtelepathyaccounts)

project (telepathy-accounts-kcm)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (telepathy_accounts_kcm_SRCS
     kcm-telepathy-accounts.cpp
     accounts-list-model.cpp
     account-item.cpp
     add-account-assistant.cpp
     protocol-select-widget.cpp
     protocol-list-model.cpp
     connection-manager-item.cpp
     protocol-item.cpp
     parameter-edit-widget.cpp
     parameter-edit-delegate.cpp
     parameter-edit-model.cpp
     parameter-item.cpp
     integer-edit.cpp
     unsigned-integer-edit.cpp
     dictionary.cpp
     plugin-manager.cpp
)

kde4_add_ui_files (telepathy_accounts_kcm_SRCS
                   main-widget.ui
                   protocol-select-widget.ui
                   parameter-edit-widget.ui
)

kde4_add_plugin (kcm_telepathy_accounts
                 ${telepathy_accounts_kcm_SRCS}
)

target_link_libraries (kcm_telepathy_accounts
                       libkcmtelepathyaccounts
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

# Install:
install (TARGETS kcm_telepathy_accounts
         DESTINATION ${PLUGIN_INSTALL_DIR}
)

install (FILES kcm_telepathy_accounts.desktop
         DESTINATION ${SERVICES_INSTALL_DIR}
)
