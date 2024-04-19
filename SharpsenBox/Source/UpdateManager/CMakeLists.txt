find_package(curl REQUIRED)
find_package(LibArchive REQUIRED)

qt_add_library(UpdateManager STATIC
        Downloader.cpp
        ArchieveInstaller.cpp
        Cleanup.cpp
        GameFileRemover.cpp
        UpdateInfo.cpp
        AppInfoParser.cpp
        FileListParser.cpp
        GameParser.cpp
        UpdateManager.cpp)

target_include_directories(UpdateManager PUBLIC
        "../QConfig/h"
        "../JsonParsers"
        "../Component/h"
        "h")

target_link_libraries(UpdateManager
        Qt6::Widgets
        Qt6::Gui
        Qt6::Core
        Qt6::Qml
        CURL::libcurl
        LibArchive::LibArchive)


if (BUILD_TESTS)
    add_subdirectory(test)
endif ()
