
find_package(Curl REQUIRED)
find_package(ZLIB REQUIRED)

conan_basic_setup(TARGETS)

add_library(InstalationManager STATIC
    InstalationManager.cpp
    FtpDownloader.cpp 
    ZlibInstaller.cpp)

target_include_directories(InstalationManager PUBLIC
    "../AppBackend"
    "../Config")

qt5_use_modules(InstalationManager 
    Quick 
    Core 
    Network)

target_link_libraries(InstalationManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl
    CONAN_PKG::zlib)