find_package(curl)
find_package(libarchieve)
conan_basic_setup(TARGETS)

add_library(InstalationManager STATIC
    InstalationManager.cpp
    FtpDownloader.cpp 
    ArchieveInstaller.cpp)

target_include_directories(InstalationManager PUBLIC
    "../AppBackend"
    "../Config"
    "../LoadingBar")

qt5_use_modules(InstalationManager 
    Quick 
    Core 
    Network)

message("asdsadsadsa d ${CURL_LIBRARIES}")
target_link_libraries(InstalationManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl
    CONAN_PKG::libarchive)