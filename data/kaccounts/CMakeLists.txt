file(GLOB service_files_in *.service.in)
foreach(service_file_in ${service_files_in})
    kaccounts_add_service(${service_file_in})
endforeach()

file(GLOB provider_in_files *.provider.in)
foreach(provider_in_file ${provider_in_files})
    kaccounts_add_provider(${provider_in_file})
endforeach()
