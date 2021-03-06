cmake_minimum_required(VERSION 2.8)

PROJECT(trans)

SET(CMAKE_BUILD_TYPE Debug)

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Krzysztof Borowczyk")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

SET(C14_ON " -std=c++14")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C14_ON}")

ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK=1)

FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Quick REQUIRED)

#SET(BOOST_ROOT "~/boost/")
#find_package(Boost 1.55.0 REQUIRED system thread log)
#if (Boost_FOUND)
#  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#  LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
#endif(Boost_FOUND)
#INCLUDE_DIRECTORIES("~/mysql_connector/include")
#LINK_DIRECTORIES("~/mysql_connector/lib")

INCLUDE_DIRECTORIES(.)

set(CMAKE_AUTOMOC ON)

SET(SOURCES
        DbAbstraction.cpp
	Importer.cpp
	Paragraph.cpp
        main.cpp
	)
        
SET(HEADERS
        IDbAbstraction.h
        DbAbstraction.h
	Importer.h
	Paragraph.h
  )


ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} Qt5::Core Qt5::Quick Qt5::Widgets pqxx pthread ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

include(CPack)
