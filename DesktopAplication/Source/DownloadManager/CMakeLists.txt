
find_package(Curl REQUIRED)

conan_basic_setup(TARGETS)

add_library(DownloadManager STATIC
    DownloadManager.cpp
    FtpDownloader.cpp)

target_include_directories(DownloadManager PUBLIC
    "../AppBackend"
    ${CURL_INCLUDE_DIRS})

qt5_use_modules(DownloadManager 
    Quick 
    Core 
    Network)

target_link_libraries(DownloadManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl
    ${CURL_LIBRARIES})