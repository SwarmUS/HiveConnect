set(LIB_NAME "swarmus-hiveconnect-network-ros")
set(LIB_ALIAS "SwarmUS::HiveConnect::Network::Platform")

set(LIB_HEADERS
        include/DummyNetworkManager.h
        include/DummyNetworkOutputStream.h
        include/DummyNetworkInputStream.h
        )

set(LIB_SOURCES
    src/NetworkContainer.cpp)


add_library(${LIB_NAME} STATIC ${LIB_HEADERS} ${LIB_SOURCES})
add_library(${LIB_ALIAS} ALIAS ${LIB_NAME})


target_include_directories(${LIB_NAME}
        PUBLIC
        include
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${NETWORK_CMAKE_DIR}/include
)

target_link_libraries(${LIB_NAME}
        PUBLIC
        SwarmUS::HiveConnect::OS
        SwarmUS::HiveConnect::Logger
        SwarmUS::Propolis::C::Common
        SwarmUS::Propolis::Pheromones::Common

        )

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)