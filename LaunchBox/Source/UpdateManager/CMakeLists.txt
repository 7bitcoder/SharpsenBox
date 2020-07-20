find_package(curl)
find_package(libarchieve)
conan_basic_setup(TARGETS)

add_library(UpdateManager STATIC
    Downloader.cpp 
    ArchieveInstaller.cpp 
    Cleanup.cpp
    GameFileRemover.cpp 
    UpdateInfo.cpp 
    AppInfoParser.cpp 
    FileListParser.cpp
    GameParser.cpp 
    UpdateManager.cpp "test/AppInfoTest.cpp" "test/main.cpp")

target_include_directories(UpdateManager PUBLIC
    "../QConfig/h"
    "../JsonParsers"
    "../Component/h"
    "h")

qt5_use_modules(UpdateManager 
    Quick 
    Core 
    Network)

target_link_libraries(UpdateManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl
    CONAN_PKG::libarchive)


if(BUILD_TESTS)
    add_subdirectory(test)
endif()