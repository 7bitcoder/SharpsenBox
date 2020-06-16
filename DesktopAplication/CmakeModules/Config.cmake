set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

option(AUTO_QT_LIBS "Runs Qt ulility program to automaticly add qt/qml libraries to bin folder" OFF)

# set basic platrofm variables
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(64BIT 1)
	set(QtLib ${Qt64Bit})
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(32BIT 1)
	set(QtLib ${Qt32Bit})
endif()

# set Qt library path
# read from file Qt paths
# filename: QtPaths.json
# format:
# {
#	"Qt32Bit": "C:\\QtTools\\5.14.2\\msvc2017",
#   "Qt64Bit": "C:\\QtTools\\5.14.2\\msvc2017_64"
# } -> see QtPathsExample.json
file(READ QtPaths.json stream)
if(64BIT)
	string(REGEX MATCH "(\"Qt64Bit\"):.?\"([^\"]*)\".?\n"  QtLib ${stream})
else()
	string(REGEX MATCH "(\"Qt32Bit\"):.?\"([^\"]*)\".?\n"  QtLib ${stream})
endif()

message("CULR dir: ${CURL_LIB}")
set(QtLib ${CMAKE_MATCH_2})
set (CMAKE_PREFIX_PATH ${QtLib}/lib/cmake/Qt5 )
message("Qt library path: ${QtLib}")


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG 1)
	set(type debug)
else()
	set(RELEASE 1)
	set(type release)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	set(WIN 1)
	set(platform win)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL  Linux)
	set(LINUX 1)
	set(platform linux)
else()
	set(MAC 1)
	set(platform mac)
endif()

set(UpdaterPath ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../Updater)
if(NOT EXISTS ${UpdaterPath})
	file(MAKE_DIRECTORY ${UpdaterPath})
endif()

set(PatherPath ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../Pather)
if(NOT EXISTS ${UpdaterPath})
	file(MAKE_DIRECTORY ${UpdaterPath})
endif()

if(AUTO_QT_LIBS)
	add_custom_target(deploy ALL
		WORKING_DIRECTORY ${QtLib}/bin 
		COMMAND ${platform}deployqt --qmldir ${CMAKE_SOURCE_DIR}/Gui --${type} --quick ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
		COMMAND ${platform}deployqt --qmldir ${CMAKE_SOURCE_DIR}/Gui --${type} --quick ${UpdaterPath}
		COMMAND ${platform}deployqt --qmldir ${CMAKE_SOURCE_DIR}/Gui --${type} --quick ${PatherPath}
		COMMENT "========= Deploying Aplication =========" 
		USES_TERMINAL)
		add_dependencies(deploy LaunchBox)
endif()

set(totalPath  ${CMAKE_SOURCE_DIR}/Config/LaunchBox)
set(pages ${CMAKE_SOURCE_DIR}/Gui)
file( COPY  ${totalPath}/LaunchBoxInfo.json ${totalPath}/Games.json DESTINATION  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../Config )
file( COPY  ${pages}/ExampleOne.qml ${pages}/ExampleTwo.qml DESTINATION  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/../Pages )

set(UpdaterPath Updater)

set(LaunchBoxPath LaunchBox)

set(PatherPath Pather)