find_package(curl)
find_package(libarchieve)
conan_basic_setup(TARGETS)

add_library(InstalationManager STATIC
    Downloader.cpp 
    ArchieveInstaller.cpp 
    Cleanup.cpp
    GameFileRemover.cpp 
    UpdateInfo.cpp 
    AppInfoParser.cpp 
    FileListParser.cpp
    GameParser.cpp 
    InstalationManager.cpp)

target_include_directories(InstalationManager PUBLIC
    "../AppBackend"
    "../AppBackend/h"
    "../ObjectRepository"
    "../Config/h"
    "../LoadingBar/h"
    "../GameManager/h"
    "../JsonParsers"
    "../AppUpdater"
    "h")

qt5_use_modules(InstalationManager 
    Quick 
    Core 
    Network)

target_link_libraries(InstalationManager
    #JsonParsers
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl
    CONAN_PKG::libarchive)