cmake_minimum_required(VERSION 3.5.1)

project(336PISC)

set(CMAKE_VERBOSE_MAKEFILE ON)

ADD_DEFINITIONS(-DUSE_WEBSOCKET -DJSONCOLUMNFUNC -DOPERMANAGE_CALLBACK_FUNC -DNO_CACHING)

file(GLOB SOURCES  *.cpp civetweb/*.cpp civetweb/*.c civetweb/*.inl commonsocket/*.cpp DataRelated/*.cpp CommHandlers/*.cpp LEDIndicator/*.cpp TablesDefine/*.cpp ScheduleManage/*.cpp rapidjson/*.cpp OperRelated/*.cpp MediaWrapper/*.cpp webPages/*.cpp)

set(CMAKE_CXX_STANDARD 11)


SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)


#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath-link,/opt/toolchain/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot/lib/arm-linux-gnueabihf")




SET(CMAKE_C_COMPILER /opt/toolchain/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /opt/toolchain/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
#SET(CMAKE_FIND_ROOT_PATH /opt/toolchain/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot)


SET(CMAKE_SYSROOT /opt/toolchain/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot)
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath,${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf")



#INCLUDE_DIRECTORIES(/opt/toolchain/gcc-linaro-5.4.1-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot/usr/include)
#LINK_DIRECTORIES(/opt/toolchain/gcc-linaro-5.4.1-2017.01zxxxxxxxxxxxxxxz-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/sysroot/lib/arm-linux-gnueabihf)
LINK_LIBRARIES(event sqlite3 c dl pthread)
 
add_executable(${PROJECT_NAME} ${SOURCES})