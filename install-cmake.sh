#!/bin/bash

baseName="TRMIDoubleCheckBox"
programName="TRMIDoubleCheckBox"

absolutePath="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
fileName="${absolutePath##*/}"
filePath="$(dirname "$absolutePath")/"

buildType="Release"

function displayHelp() {
    echo "Usage: install-$programName.sh [--install/--uninstall] [build-dir]"
}

function cleanUp() {
    echo "All cleaned up"
}

function runCmake() {
    if [[ -z "$cmakeInstallLocation" ]]; then
        cmakeInstallLocation="-DCMAKE_INSTALL_PREFIX=/usr/"
    fi
    echo "Running cmake (BuildType = $buildType) from source directory \"$1\""
    if [[ "$verboseOutput" == "1" ]]; then
        verboseOutputArgs="-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
    else
        verboseOutputArgs=""
    fi
    msysCheck=$(uname -a | grep -i 'msys')
    if [[ ! -z "$msysCheck" ]]; then
        mingwRoot="/c/Qt/Tools/mingw530_32/bin"
        isMSYS="-DCMAKE_MAKE_PROGRAM=${mingwRoot}/mingw32-make.exe -DCMAKE_C_COMPILER=${mingwRoot}/gcc.exe -DCMAKE_CXX_COMPILER=${mingwRoot}/g++.exe -DMINGW:BOOL=ON"
    else
        isMSYS=""
    fi
    echo -n "Running command: \"cmake -DSTATIC_BUILD:BOOL=ON $cmakeVariables $cmakeInstallLocation $verboseOutputArgs $isMSYS -DCMAKE_BUILD_TYPE=$buildType \"$1\"..."
    cmake -DSTATIC_BUILD:BOOL=ON $cmakeVariables $cmakeInstallLocation $verboseOutputArgs $isMSYS -DCMAKE_BUILD_TYPE=$buildType "$1"
}

function bailout() {
    rm -rf "$buildDir"
}

buildDir="build"
appDir="$HOME/.local/share/applications/"

if [[ "$EUID" -eq "0" ]]; then
    echo "This script must NOT be run as root"
    exit 1
    SUDO=
else
    SUDO=sudo
fi

trap bailout INT QUIT TERM
verboseOutput=0

declare -i loopCounter
loopCounter=0
for var in "$@"; do
    if [[ "$var" == "-r" || "$var" == "--r" || "$var" == "-release" || "$var" == "--release" ]]; then
        buildType="Release"
    elif [[ "$var" == "-d" || "$var" == "--d" || "$var" == "-debug" || "$var" == "--debug" ]]; then
        buildType="Debug"
    elif [[ "$var" == "-v" || "$var" == "--v" || "$var" == "-verbose" || "$var" == "--verbose" ]]; then
        verboseOutput=1
    elif [[ "$var" == -DCMAKE_INSTALL_PREFIX* ]]; then
        cmakeInstallLocation="$var"
    elif [[ "$var" == -D* ]]; then
        cmakeVariables="$cmakeVariables $var"
    fi
    loopCounter=$((loopCounter+1))
done

if [[ $# -gt 0 ]]; then
    var=""
    buildDir="$filePath/$buildDir"
    for var in "$@"; do
        if [[ $var == -* ]]; then
            continue
        fi
        buildDir="$var"
    done
else
    buildDir="$filePath/$buildDir"
fi
if ! [[ -d "$buildDir" ]]; then
    mkdir "$buildDir" || { echo "Unable to make build directory \"$buildDir\""; exit 1; }
fi

cd "$buildDir" || { echo "Unable to enter build directory \"$buildDir\""; exit 1; }
runCmake "$filePath" || { echo "cmake failed"; exit 1; }
make -j$(nproc) || { echo "make failed, bailing out"; exit 1; }
$SUDO make install || { echo "Could not install"; exit 1; }
$SUDO chown -R $USER:$USER "$HOME/Desktop/$programName.desktop" || { echo "Could not mark desktop file executable, bailing out"; exit 1; }
$SUDO chmod a+rwx "$HOME/Desktop/$programName.desktop" || { echo "Could not mark desktop file executable, bailing out"; exit 1; }

installMessage="$programLongName Installed Successfully!"
totalLength=${#installMessage} 

echo
for (( i=0; i<totalLength+4; i++ )); do  
   echo -n "*"
done
echo
echo "**$installMessage**"
for (( i=0; i<totalLength+4; i++ )); do  
   echo -n "*"
done
echo
echo
exit 0
