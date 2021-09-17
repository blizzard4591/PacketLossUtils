# PacketLossUtils
A cross-platform utility for analyzing packet loss between hosts using UDP packets.

## License
PacketLossUtils are governed by the GNU GPL v2.0 license. See [`LICENSE`](LICENSE) for more information.

PacketLossUtils make use of the wonderful [SPDLOG](https://github.com/gabime/spdlog), a logging library, licensed under the MIT license.

## Requirements
 - [CMake](https://cmake.org/)
 - [Qt 5](https://www.qt.io/)

## Supported Platforms
Currently, the application has been built and tested on:
 - Windows 10 using Visual Studio 2019
 - Debian 10/11 using GCC 9/10 (AMD64)
 - Debian 10 using GCC 8.3 (armv7l)
 
Other platforms should work with no or minimal changes.

## How to use
Select one host as the server. On this host, start the utility using `packetLossUtils --server`.
On all other hosts (clients), start the utility using `packetLossUtils --client your.servers.hostname.org`.

## How to build

Find detailed steps for Linux and Windows systems below.

### General steps

1. Clone this repository or extract the sources from the archive.  
	If you clone the repository, do not forget to intialize the submodules (i.e. external git repositories):

		git submodule init
		git submodule sync
		git submodule update --init --recursive

2. Run CMake on the main directory.
4. Once Makefiles or a solution (MSVS) has been successfully generated, start the build process.  
	On Windows, choosing a `DEBUG` configuration causes the application to show additional run-time debug information and logs.
    

Prerequisites on Debian/Ubuntu: 
```
apt-get install libqt5core5a qt5-qmake qtbase5-dev qtbase5-dev-tools git g++ make cmake
```

### Detailed steps – Linux

Adjust the paths to your system. The ones below are a current Qt version installed on OSX via brew.
```bash
cd my-projects/ # or wherever you want to clone this

# ONLY NECESSARY FOR MacOS - If you are on Mac OSX (install Qt5 using Homebrew, and then find the suitable paths for your installation)
export Qt5Core_DIR=/usr/local/Cellar/qt5/5.7.0/lib/cmake/Qt5Core/
export Qt5Gui_DIR=/usr/local/Cellar/qt5/5.7.0/lib/cmake/Qt5Gui/
export Qt5Widgets_DIR=/usr/local/Cellar/qt5/5.7.0/lib/cmake/Qt5Widgets/
export Qt5Network_DIR=/usr/local/Cellar/qt5/5.7.0/lib/cmake/Qt5Network/
export Qt5Multimedia_DIR=/usr/local/Cellar/qt5/5.7.0/lib/cmake/Qt5Multimedia

# Build PacketLossUtils
git clone https://github.com/blizzard4591/PacketLossUtils.git
cd PacketLossUtils
mkdir build
cd build

cmake .. # or "cmake .. -G Xcode" for MacOSX, optionally add " -DCMAKE_BUILD_TYPE=Debug" for a debug build
make # optionally add " -j4" for multi-threaded compilation with 4 threads
./packetLossUtils --help
```


### Detailed steps – Windows

1. Install [CMake](https://cmake.org/download/)
2. Download and install the [latest Qt version](https://www.qt.io/download-open-source/). Check that you select the right version for your version of Visual Studio, for example msvc-2019-x64 for Visual Studio 2019 and 64bit builds.
5. Open the `CMakeLists.txt` file in the root folder of openMittsu and look for `PACKETLOSSUTILS_CMAKE_SEARCH_PATH`.  
	Edit the path to point to your Qt installation.
6. Start CMake and point it to the PacketLossUtils directory.
	Lets say you cloned PacketLossUtils to `C:\cpp\PacketLossUtils`, then fill out the two lines in CMake as follows:
		
        Where is the source code: C:/cpp/PacketLossUtils
		Where to build the binaries: C:/cpp/PacketLossUtils/build
        
	Using the `/build` folder allows us to perform an out-of-source build that does not pollute the sources with build files.  
	You can even have simultaneous x86 and x64 builds from the same source, just by using different build directories.  
	Click `Configure` and select the appropriate generator for Visual Studio, for example `Visual Studio 16 2019 Win64` for Visual Studio 2019 in 64bit mode.  
	Click `Generate` and open the generated solution file in the build directory.  
	You can now build either a Debug or a Release build of PacketLossUtils by using the standard Visual Studio target selection.

