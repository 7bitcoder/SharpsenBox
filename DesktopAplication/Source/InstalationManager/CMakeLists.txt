


find_package(CURL REQUIRED)
find_package(libcurl REQUIRED)
conan_basic_setup(InstalationManager)

add_library(InstalationManager STATIC
    InstalationManager.cpp
    FtpDownloader.cpp 
    ZlibInstaller.cpp)


target_include_directories(InstalationManager PUBLIC
    "../AppBackend"
    "../Config"
    ${LCURL_INC})

qt5_use_modules(InstalationManager 
    Quick 
    Core 
    Network)

message("=====------ ${LCURL_LIBRARY}")
target_link_libraries(InstalationManager
    ${CURL_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    libcurl::libcurl)