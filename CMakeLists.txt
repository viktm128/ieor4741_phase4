cmake_minimum_required(VERSION 3.28)
project(phase4)

set(CMAKE_CXX_STANDARD 20)

# Tick Processing Project
#include_directories(tick_processing)
#add_executable(phase4
#        main_tick.cpp
#)

include_directories(include)
add_executable(phase4
        main_hft.cpp
        # src/MarketData.cpp
        #src/OrderBook.cpp
        #src/MatchingEngine.cpp
        #src/TradeLogger.cpp
)