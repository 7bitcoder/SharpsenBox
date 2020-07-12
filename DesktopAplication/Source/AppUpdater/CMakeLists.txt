add_library( AppUpdaterBackend STATIC
    AppUpdater.cpp )

target_include_directories(AppUpdaterBackend PUBLIC
    "../MainWindow" 
    "../AppBackend"
    "../InstalationManager"
    "../Config")

qt5_use_modules(AppUpdaterBackend 
    Quick 
    Core)

target_link_libraries(AppUpdaterBackend
    LoadingBar
    GameManager
    Dialog
    InstalationManager
    Config
    Qt5::Gui
    Qt5::Core)
