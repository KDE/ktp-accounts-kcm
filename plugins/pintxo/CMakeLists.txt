project (ktpaccountskcm-plugin-pintxo)

INCLUDE(FindPkgConfig)

pkg_check_modules(LIBMODEMMANAGERQT ModemManagerQt=0.5.1)
if (LIBMODEMMANAGERQT_FOUND)
    MESSAGE("ModemManagerQt >= 0.5.1 found, building pintxo plugin for SMS messaging")
    include_directories (${CMAKE_CURRENT_BINARY_DIR}
                        ${CMAKE_CURRENT_SOURCE_DIR}
                        ${QT_INCLUDES}
                        ${LIBMODEMMANAGERQT_INCLUDE_DIRS}
                        ${CMAKE_INCLUDE_PATH}
    )

    link_directories(${LIBMODEMMANAGERQT_LIBRARY_DIRS})

    set (ktpaccountskcm_plugin_pintxo_SRCS
        pintxo-account-ui-plugin.cpp
        pintxo-account-ui.cpp
        main-options-widget.cpp
        modem-combobox.cpp
    )

    kde4_add_ui_files (ktpaccountskcm_plugin_pintxo_SRCS
                    main-options-widget.ui
    )

    kde4_add_plugin (ktpaccountskcm_plugin_pintxo
                    ${ktpaccountskcm_plugin_pintxo_SRCS}
    )


    target_link_libraries (ktpaccountskcm_plugin_pintxo
                        ktpaccountskcminternal
                        ModemManagerQt
                        ${QT_LIBRARIES}
                        ${KDE4_KDEUI_LIBS}
                        ${TELEPATHY_QT4_LIBRARIES}
    )

    # Install:
    install (TARGETS ktpaccountskcm_plugin_pintxo
            DESTINATION ${KDE_INSTALL_PLUGINDIR}
    )

    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/ktpaccountskcm_plugin_pintxo.desktop.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_pintxo.desktop
                @ONLY)

    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/ktpaccountskcm_plugin_pintxo.desktop
            DESTINATION ${KDE_INSTALL_KSERVICES5DIR}
    )

endif (LIBMODEMMANAGERQT_FOUND)
