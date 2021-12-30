add_custom_command(
    OUTPUT add_u.h add_u.c add_args.h
    DEPENDS ${CMAKE_SOURCE_DIR}/add.edl
    COMMAND
     openenclave::oeedger8r --untrusted ${CMAKE_SOURCE_DIR}/add.edl
     --search-path ${OE_INCLUDEDIR} --search-path
     ${OE_INCLUDEDIR}/openenclave/edl/sgx)

add_executable(add_host host.c
        ${CMAKE_CURRENT_BINARY_DIR}/add_u.c)

target_include_directories(
    add_host PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(add_host openenclave::oehost)