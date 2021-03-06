set(LIB_NAME "swarmus-hiveconnect-os")
set(LIB_ALIAS "SwarmUS::HiveConnect::OS")

set(LIB_SOURCES
 )

set(LIB_HEADERS
)

add_library(${LIB_NAME} STATIC ${LIB_SOURCES} ${LIB_HEADERS})
add_library(${LIB_ALIAS} ALIAS ${LIB_NAME})

# Wraps the interface library in a static library
get_target_property(PROPOLIS_OS_INCLUDES SwarmUS::Propolis::OS::POSIX INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(${LIB_NAME}
    PUBLIC
       ${PROPOLIS_OS_INCLUDES}
)

target_link_libraries(${LIB_NAME}
    PRIVATE
        SwarmUS::Propolis::OS::POSIX
        libpthread.so.0
    PUBLIC
        SwarmUS::Propolis::OS::Common
)


# Removing warnings from freertos compilation on executable target
if (DISABLE_EXTERNAL_WARNINGS) 
    target_compile_options(${LIB_NAME} PRIVATE -w)
    set_target_properties(${LIB_NAME} PROPERTIES CXX_CLANG_TIDY "" )
    set_target_properties(${LIB_NAME} PROPERTIES C_CLANG_TIDY "" )
endif()
