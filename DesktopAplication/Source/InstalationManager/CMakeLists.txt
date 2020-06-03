add_library(InstalationManager STATIC
    InstalationManager.cpp
    FtpDownloader.cpp 
    ZlibInstaller.cpp)

target_include_directories(InstalationManager PUBLIC
    "../AppBackend"
    "../Config"
    "C:/Users/Sylwek/Desktop/LaunchBox/DesktopAplication/vcpkg/packages/curl_x86-windows-static/include")

qt5_use_modules(InstalationManager 
    Quick 
    Core 
    Network)

message("asdsadsadsa d ${CURL_LIBRARIES}")
target_link_libraries(InstalationManager
    ${CURL_LIBRARIES}
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core)