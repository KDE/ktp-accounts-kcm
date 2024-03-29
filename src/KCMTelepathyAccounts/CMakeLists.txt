project (libktpaccountskcminternal)

add_definitions (-DMAKE_KCM_TELEPATHY_ACCOUNTS_LIB)

set (libktpaccountskcminternal_SRCS
     abstract-account-ui-plugin.cpp
     abstract-account-ui.cpp
     abstract-account-parameters-widget.cpp
     generic-advanced-options-widget.cpp
     account-edit-widget.cpp
     profile-select-widget.cpp
     simple-profile-select-widget.cpp
     profile-list-model.cpp
     profile-item.cpp
     parameter-edit-widget.cpp
     parameter-edit-delegate.cpp
     parameter-edit-model.cpp
     parameter-item.cpp
     dictionary.cpp
     plugin-manager.cpp
     feedback-widget.cpp
     validated-line-edit.cpp
)

set (libktpaccountskcminternal_HDRS
     abstract-account-ui-plugin.h
     abstract-account-ui.h
     abstract-account-parameters-widget.h
     generic-advanced-options-widget.h
     account-edit-widget.h
     plugin-manager.h
     profile-list-model.h
     profile-select-widget.h
     simple-profile-select-widget.h
     profile-item.h
     parameter-edit-widget.h
     parameter-edit-model.h
     validated-line-edit.h
)

set (libktpaccountskcminternal_PRETTY_HDRS
     AbstractAccountUiPlugin
     AbstractAccountUi
     AbstractAccountParametersWidget
     GenericAdvancedOptionsWidget
     AccountEditWidget
     ConnectionManagerItem
     PluginManager
     ProtocolSelectWidget
     ProtocolItem
     ParameterEditWidget
     ParameterEditModel
     ValidatedLineEdit
)

ki18n_wrap_ui (libktpaccountskcminternal_SRCS
               account-edit-widget.ui
               parameter-edit-widget.ui
               profile-select-widget.ui
               simple-profile-select-widget.ui
)

add_library(ktpaccountskcminternal
            SHARED
            ${libktpaccountskcminternal_SRCS}
)

if (TARGET TelepathyQt5::Core) # telepathy-qt 0.9.8
    set (TELEPATHY_QT5_LIBRARIES TelepathyQt5::Core)
endif()

target_link_libraries (ktpaccountskcminternal
                       Qt5::Widgets
                       Qt5::Core
#                        KF5::KIO
                       KF5::I18n
                       KF5::WidgetsAddons
                       KF5::ConfigWidgets
                       KF5::ItemViews
                       KF5::IconThemes
                       KF5::Service
                       ${TELEPATHY_QT5_LIBRARIES}
)


set_target_properties (ktpaccountskcminternal PROPERTIES
                       VERSION ${KTP_ACCOUNTS_KCM_VERSION}
                       SOVERSION ${KTP_ACCOUNTS_KCM_SONUMBER}
)

if (MSVC)
    set_target_properties (ktpaccountskcminternal PROPERTIES
                           OUTPUT_NAME libktpaccountskcminternal
    )
endif (MSVC)

# Install:
install (TARGETS ktpaccountskcminternal
         ${KDE_INSTALL_TARGETS_DEFAULT_ARGS}
         LIBRARY NAMELINK_SKIP
)

install (FILES
         ktpaccountskcminternal-accountuiplugin.desktop
         DESTINATION ${KDE_INSTALL_KSERVICETYPES5DIR}
)

