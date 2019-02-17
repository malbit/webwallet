cmake_minimum_required(VERSION 3.2)

project(myxrm)

set(SOURCE_FILES
    CmdLineOptions.cpp
    CurrentBlockchainStatus.cpp
    db/MySqlConnector.cpp
    db/MySqlAccounts.cpp
    db/ssqlses.cpp
    ArqmaRequests.cpp
    TxSearch.cpp
    RPCCalls.cpp
    arqversion.h.in
    BlockchainSetup.cpp
    ThreadRAII.cpp
    db/MysqlPing.cpp
    TxUnlockChecker.cpp
    UniversalIdentifier.cpp
    RandomOutputs.cpp)

# make static library called libmyxrm
# that we are going to link to
# in the root CMakeLists.txt file
add_library(myxrm
		STATIC
		${SOURCE_FILES})

target_include_arqma_directories(myxrm)
target_include_directories(myxrm PRIVATE "arqmacore")

    #target_include_directories(myxmr
    #  PUBLIC
    #${ARQMA_SOURCE_DIR}/contrib/epee/include)
