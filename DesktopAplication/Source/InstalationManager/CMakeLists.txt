find_package(curl)
find_package(libarchieve)
conan_basic_setup(TARGETS)

add_library(InstalationManager STATIC
    InstalationManager.cpp
    Downloader.cpp 
    ArchieveInstaller.cpp 
    Cleanup.cpp
    GameFileRemover.cpp 
    UpdateInfo.cpp 
    AppInfoParser.cpp 
    FileListParser.cpp
    GameParser.cpp "IInstalationManager.hpp" "IRunnable.hpp")

target_include_directories(InstalationManager PUBLIC
    "./h"
    "../AppBackend"
    "../Config"
    "../LoadingBar"
    "../GameManager"
    "../JsonParsers")

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