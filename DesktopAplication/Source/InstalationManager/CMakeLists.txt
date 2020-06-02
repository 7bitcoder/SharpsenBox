
add_library(InstalationManager STATIC
    InstalationManager.cpp
    FtpDownloader.cpp 
    ZlibInstaller.cpp)

    message("=====------ ${CURL_INC}")

target_include_directories(InstalationManager PUBLIC
    "../AppBackend"
    "../Config"
    ${CURL_INC})

qt5_use_modules(InstalationManager 
    Quick 
    Core 
    Network)

target_link_libraries(InstalationManager
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    ${CURL_LIBRARY})