cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake/modules ${CMAKE_MODULE_PATH})

find_package (AccountsFileDir REQUIRED)
find_package (Intltool REQUIRED) #needed to generate service and provider files

include(FeatureSummary)

file(GLOB service_files_in *.service.in)
foreach(service_file_in ${service_files_in})
    get_filename_component(service_filename ${service_file_in} NAME_WE)
    set(service_file ${CMAKE_CURRENT_BINARY_DIR}/${service_filename}.service)
    execute_process(COMMAND intltool-merge -x -u --no-translations ${service_file_in} ${service_file})
    install(FILES ${service_file} DESTINATION ${ACCOUNTS_SERVICES_DIR})
endforeach()

file(GLOB provider_in_files *.provider.in)
foreach(provider_in_file ${provider_in_files})
    get_filename_component(provider_filename ${provider_in_file} NAME_WE)
    set(provider_file ${CMAKE_CURRENT_BINARY_DIR}/${provider_filename}.provider)
    execute_process(COMMAND intltool-merge -x -u --no-translations ${provider_in_file} ${provider_file})
    install(FILES ${provider_file} DESTINATION ${ACCOUNTS_PROVIDERS_DIR})
endforeach()
