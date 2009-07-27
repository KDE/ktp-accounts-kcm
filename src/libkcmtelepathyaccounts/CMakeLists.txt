project (libkcmtelepathyaccounts)

include_directories (${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
)

set (libkcmtelepathyaccounts_SRCS
     abstract-account-ui-plugin.cpp
)

set (libkcmtelepathyaccounts_HDRS
     abstract-account-ui-plugin.h
)

set (libkcmtelepathyaccounts_PRETTY_HDRS
     include/AbstractAccountUiPlugin
)

kde4_add_library (libkcmtelepathyaccounts
                  SHARED
                  ${libkcmtelepathyaccounts_SRCS}
)

target_link_libraries (libkcmtelepathyaccounts
                       ${QT_LIBRARIES}
                       ${KDE4_KDEUI_LIBS}
                       ${TELEPATHY_QT4_LIBRARIES}
)

set_target_properties (libkcmtelepathyaccounts PROPERTIES
                       VERSION ${GENERIC_LIB_VERSION}
                       SOVERSION ${GENERIC_LIB_SOVERSION}
)

if (MSVC)
    set_target_properties (libkcmtelepathyaccounts PROPERTIES
                           OUTPUT_NAME libkcmtelepathyaccounts
    )
endif (MSVC)

# Install:
install (TARGETS libkcmtelepathyaccounts
         ${INSTALL_TARGETS_DEFAULT_ARGS}
)

install (FILES
         ${libkcmtelepathyaccounts_HDRS}
         ${libkcmtelepathyaccounts_PRETTY_HDRS}
         DESTINATION ${INCLUDE_INSTALL_DIR}/KCMTelepathyAccounts COMPONENT Devel
)

