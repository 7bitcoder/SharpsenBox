
conan_basic_setup(TARGETS)

add_library(BottomBar STATIC
    BottomBar.cpp
    FtpDownloader.cpp)

target_include_directories(BottomBar PUBLIC
"../AppBackend")

qt5_use_modules(BottomBar 
    Quick 
    Core 
    Network)

target_link_libraries(BottomBar
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    CONAN_PKG::libcurl )