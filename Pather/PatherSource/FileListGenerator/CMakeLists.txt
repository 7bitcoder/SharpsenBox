
add_executable(FileListGenerator
    main.cpp "Generator.cpp" "Generator.hpp")

qt5_use_modules(FileListGenerator 
    Quick 
    Core 
    Network)

target_link_libraries(FileListGenerator
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Quick
    Qt5::Qml
    Qt5::QuickControls2)

install(TARGETS FileListGenerator DESTINATION ${LaunchBoxPath}/bin)
install(TARGETS FileListGenerator DESTINATION ${PatherPath}/Pather)