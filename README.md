# LaunchBox
LaunchBox is free game launcher, you can use this code and modify for your project. Main problems are solved such as version checking, reading your game web page, showing tips, etc. LaunchBox is written in qt/c++ so program is multiplatform

System contains two main programs: Patcher and LaunchBox. Patcher is developer tool for generating game patches. (zip packets, json files). LaunchBox is client program, that allows to install your game, update, uninstall, autoupdate wchen never verion is available. Witch patcher you can split your game files into several packets (zip files). I suggest you to separate library/game engine files, assets, executable files. When newer version is available LaunchBox dowloads packets which contains modified/new files and unpack them to game directory. LauchBox also can delete files that are not neede any more. You dont need any server to comute delta between current client game version and actual, everything is computed on client side in LaunchBox.
# Libraries
## Curl - use conan to install
## QT - path in qt paths
