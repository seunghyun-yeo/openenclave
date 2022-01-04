set(headers ${CMAKE_SOURCE_DIR}/../../include)
add_custom_command(
    OUTPUT add_t.h add_t.c add_args.h
    DEPENDS ${CMAKE_SOURCE_DIR}/add.edl
    COMMAND
        openenclave::oeedger8r --trusted --migratable ${CMAKE_SOURCE_DIR}/add.edl
        --search-path ${OE_INCLUDEDIR} --search-path 
        ${OE_INCLUDEDIR}/openenclave/edl/sgx)

add_executable(enclave enc.c ${CMAKE_CURRENT_BINARY_DIR}/add_t.c)

target_compile_definitions(enclave PUBLIC OE_API_VERSION=2)

target_include_directories(enclave PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(enclave PUBLIC ${headers})

target_link_libraries(
    enclave openenclave::oeenclave openenclave::oecrypto${OE_CRYPTO_LIB}
    openenclave::oelibc)