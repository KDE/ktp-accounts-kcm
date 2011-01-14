project (libkcmtelepathyaccounts)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (libkcmtelepathyaccounts_SRCS
     abstract-account-ui-plugin.cpp
     abstract-account-ui.cpp
     abstract-account-parameters-widget.cpp
     generic-advanced-options-widget.cpp
     account-edit-widget.cpp
     profile-select-widget.cpp
     profile-list-model.cpp
     profile-item.cpp
     parameter-edit-widget.cpp
     parameter-edit-delegate.cpp
     parameter-edit-model.cpp
     parameter-item.cpp
     dictionary.cpp
     plugin-manager.cpp
     protocol-parameter-value.cpp
)

set (libkcmtelepathyaccounts_HDRS
     abstract-account-ui-plugin.h
     abstract-account-ui.h
     abstract-account-parameters-widget.h
     generic-advanced-options-widget.h
     account-edit-widget.h
     plugin-macros.h
     plugin-manager.h
     profile-select-widget.h
     profile-item.h
     parameter-edit-widget.h
     parameter-edit-model.h
     protocol-parameter-value.h
)

set (libkcmtelepathyaccounts_PRETTY_HDRS
     include/AbstractAccountUiPlugin
     include/AbstractAccountUi
     include/AbstractAccountParametersWidget
     include/GenericAdvancedOptionsWidget
     include/AccountEditWidget
     include/PluginMacros
     include/ConnectionManagerItem
     include/PluginManager
     include/ProtocolSelectWidget
     include/ProtocolItem
     include/ParameterEditWidget
     include/ParameterEditModel
     include/ProtocolParameterValue
)

kde4_add_ui_files (libkcmtelepathyaccounts_SRCS
                   account-edit-widget.ui
                   parameter-edit-widget.ui
                   profile-select-widget.ui
)

kde4_add_library (kcmtelepathyaccounts
                  SHARED
                  ${libkcmtelepathyaccounts_SRCS}
)

target_link_libraries (kcmtelepathyaccounts
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

set_target_properties (kcmtelepathyaccounts PROPERTIES
                       VERSION ${GENERIC_LIB_VERSION}
                       SOVERSION ${GENERIC_LIB_SOVERSION}
)

if (MSVC)
    set_target_properties (kcmtelepathyaccounts PROPERTIES
                           OUTPUT_NAME libkcmtelepathyaccounts
    )
endif (MSVC)

# Install:
install (TARGETS kcmtelepathyaccounts
         ${INSTALL_TARGETS_DEFAULT_ARGS}
)

install (FILES
         ${libkcmtelepathyaccounts_HDRS}
         ${libkcmtelepathyaccounts_PRETTY_HDRS}
         DESTINATION ${INCLUDE_INSTALL_DIR}/KCMTelepathyAccounts COMPONENT Devel
)

install (FILES
         kcmtelepathyaccounts-accountuiplugin.desktop
         DESTINATION ${SERVICETYPES_INSTALL_DIR}
)

