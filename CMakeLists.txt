cmake_minimum_required(VERSION 3.10)

project(executor C CXX)

include_directories("${CMAKE_SOURCE_DIR}/inc")
#LINK_DIRECTORIES("/usr/local/lib")
SET(CMAKE_FLAGS_BASE "$ENV{CXXFLAGS} -std=c++11 -Wall 
-pipe 
-fpermissive 
-fPIC
-Wunreachable-code 
-march=native 
-Wno-unused-parameter 
-Wno-unused-function 
-Wunused-variable 
-Wunused-value 
-Wcast-qual 
-Wcast-align 
-Wwrite-strings 
-Wsign-compare 
-Winvalid-pch 
-fms-extensions 
-Wno-sign-compare 
-Wno-unused-local-typedefs 
-Winit-self 
-Wfloat-equal 
-Wextra 
-D__const__=")

find_package(Threads)

ADD_SUBDIRECTORY(src)

file(GLOB_RECURSE SOURCES "main.cpp"
	"src/Executor.cpp"
	"src/TaskThread.cpp"
	"src/ThreadPool.cpp"
    "src/ThreadMoniker.cpp"
    "src/WorkerThread.cpp"
	)
add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(${PROJECT_NAME})


#SET(CMAKE_INSTALL_PREFIX "/usr/local/speech")


#编译产物安装配置
#install(TARGETS ${PROJECT_NAME}
#    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
#)

#install(DIRECTORY "${CMAKE_SOURCE_DIR}/conf" DESTINATION "${CMAKE_INSTALL_PREFIX}")
#install(DIRECTORY "${CMAKE_SOURCE_DIR}/data" DESTINATION "${CMAKE_INSTALL_PREFIX}")

