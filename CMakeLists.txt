cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME
        arqwallet)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_SIZEOF_VOID_P EQUAL "4")
  add_definitions(-DMDB_VL32)
endif()

if (NOT ARQMA_DIR)
    set(ARQMA_DIR ~/arqma)
endif()

option(BUILD_TESTS "Build tests for the project" OFF)

message(STATUS ARQMA_DIR ": ${ARQMA_DIR}")

set(ARQMA_SOURCE_DIR ${ARQMA_DIR}
        CACHE PATH "Path to the root directory for ARQMA")

# set location of ARQMA build tree
set(ARQMA_BUILD_DIR ${ARQMA_SOURCE_DIR}/build/Linux/release-v0.2.1/release/
        CACHE PATH "Path to the build directory for ARQMA")

set(MY_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake"
        CACHE PATH "The path to the cmake directory of the current project")

list(APPEND CMAKE_MODULE_PATH "${MY_CMAKE_DIR}")

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${ARQMA_BUILD_DIR}"
        CACHE PATH "Add ARQMA directory for library searching")

include(MyUtils)


if (${BUILD_TESTS})
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_EXCLUDES
            '${CMAKE_SOURCE_DIR}/ext/*'
            'asio/*'
            '${ARQMA_SOURCE_DIR}/*'
            '${CMAKE_SOURCE_DIR}/tests/*')
endif()

find_package(Arqma)

include_directories(/usr/include/mysql)

# include boost headers
include_directories(${Boost_INCLUDE_DIRS})

# include ARQMA headers
include_directories("ext/restbed/source")

include_directories(
    ${ARQMA_SOURCE_DIR}/build/Linux/release-v0.2.1
    /usr/local/include
    /usr/include/mysql
    /usr/local/opt/openssl/include
)

link_directories(
    ${ARQMA_BUILD_DIR}/src/crypto
    /usr/local/lib
    /usr/local/opt/openssl/lib
)

create_git_version()

configure_files(${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_BINARY_DIR}/config)


# find boost
find_package(Boost COMPONENTS
        system
        filesystem
        thread
        date_time
        chrono
        regex
        serialization
        program_options
        date_time
        REQUIRED)


# add src/ subfolder
add_subdirectory(src/)

# add ext/ subfolder
add_subdirectory(ext/)


set(SOURCE_FILES
        main.cpp)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES})

set(LIBRARIES
        myxrm
        myext
        restbed
        wallet
        cryptonote_core
        blockchain_db
        cryptonote_protocol
        cryptonote_basic
        cncrypto
        lmdb
        ringct
        ringct_basic
        common
        mnemonics
        epee
        easylogging
        mysqlpp
        mysqlclient
        device
        epee
        checkpoints
        version
        sodium
        ${Boost_LIBRARIES}
        pthread
        unbound
        curl
        cncrypto
        ssl
        crypto)

if(APPLE)
    set(LIBRARIES ${LIBRARIES} "-framework IOKit -framework Foundation")
else()
    set(LIBRARIES ${LIBRARIES} atomic)
endif()

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT WIN32)
    set(LIBRARIES ${LIBRARIES} unwind)
endif()


if (WIN32)
    set(LIBRARIES ${LIBRARIES}
            wsock32
            ntdll
            ws2_32
            Iphlpapi)
else()
    set(LIBRARIES ${LIBRARIES} dl)
endif()

find_package(HIDAPI)
set(LIBRARIES ${LIBRARIES} ${HIDAPI_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})


if (${BUILD_TESTS})
    include_directories(
        ${CMAKE_SOURCE_DIR}/ext/googletest/googletest/include
        ${CMAKE_SOURCE_DIR}/ext/googletest/googlemock/include)
endif()

configure_files(${CMAKE_CURRENT_SOURCE_DIR}/sql ${CMAKE_CURRENT_BINARY_DIR}/sql)

if (${BUILD_TESTS})
    enable_testing()
    add_subdirectory(ext/googletest)
    add_subdirectory(tests)
endif()
