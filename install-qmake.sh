
#!/bin/bash

##########################################
# install-qserialterminal-qmake.sh
#
# Intended to be a placeholder until
# I learn how to add a sudo make
# install/uninstall to CMake
#
##########################################

baseName="qserialterminal"
programLongName="QSerialTerminal"
programName="QSerialTerminal"
iconName="$baseName.png"
skeletonDesktopFileName=".$baseName.desktop.skel"
desktopFileName="$baseName.desktop"

absolutePath="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/$(basename "${BASH_SOURCE[0]}")"
fileName="${absolutePath##*/}"
filePath="$(dirname "$absolutePath")/"
sourceDir="$filePath/src/"
utilityDir="$filePath/utility/"
resourceDir="$filePath/resources/"
iconPath="$resourceDir/$iconName"
globalBinDir="/usr/bin/"
dependancyLibName="lib$dependancyRootName.so"
libDir="/usr/lib/"
dependancy="$libDir/$dependancyLibName"
dependancyRootName="tjlutils"
dependancySource="https://github.com/Pinguinsan/$dependancyRootName"
dependancyInstallCommand="$dependancyRootName-install.sh"

function bailout() {
    rm -rf "$buildDir"
}

function displayHelp() {
    echo "Usage: install-$programName.sh [--install/--uninstall] [build-dir]"
}

function doInternetCheck() {

    echo -n "Checking for Cygwin..."
    cygwinCheck=$(uname -a | grep -i 'cygwin')
    showSuccess

    echo -n "Checking for internet connection..."
    if [[ -z "$cygwinCheck" ]]; then
        internetCheck=$(ping 8.8.8.8 -n1 | grep -i 'received = 1')
    else
        internetCheck=$(ping 8.8.8.8 -c1 -W1 | grep -i '1 received')
    fi

    if [[ -z "$internetCheck" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function doCygwinCheck() {
    echo -n "Checking for Cygwin..."
    cygwinCheck=$(uname -a | grep -i 'cygwin')
    showSuccess

    echo -n "Setting sudo to correct value..."
    if [[ -z "$cygwinCheck" ]]; then
        SUDO="sudo"
    else
        SUDO=""
    fi
    showSuccess
}

function cleanUp() {
    echo "All cleaned up"
}

function showSuccess() {
    echo "success"
}

function showFailure() {
    echo "failure"
    cleanUp
}

function removeFile() {
    echo -n "Removing \"$1\"..."
    rm -f "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function linkFile() {
    echo -n "Linking \"$1\" to \"$2\"..."
    ln -s -f "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function suLinkFile() {
    echo -n "Linking \"$1\" to \"$2\"..."
    $SUDO ln -s -f "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function copyFile() {
    echo -n "Copying \"$1\" to \"$2\"..."
    cp -R "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function suCopyFile() {
    echo -n "Copying \"$1\" to \"$2\"..."
    $SUDO cp -R "$1" "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function appendStringToFile() {
    echo -n "Putting string \"$1\" into file $2..."
    echo "$1" >> "$2"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function changeDirectory() {
    echo -n "Entering directory \"$1\"..."
    cd "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function createDirectory() {
    echo -n "Creating directory \"$1\"..."
    mkdir -p "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function runGitClone() {
    echo -n "Cloning \"$1\" using git..."
    git clone "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function runGitPullOrigin() {
    echo -n "Fetching source for \"$1\" using git..."
    git pull origin master
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function runCmake() {
    echo -n "Running cmake from source directory \"$1\"..."
    cmake "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function runQmake() {
    echo -n "Running qmake from source directory \"$1\"..."
    qmake "$1"
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function runMake() {
    echo -n "Running make..."
    make
    if [[ "$?" -ne "0" ]]; then
        showFailure
        return 1
    else
        showSuccess
        return 0
    fi
}

function bailout() {
    rm -rf "$buildDir"
}

function buildDependancy() {
     source "$dependancyDir/$dependancyRootName/$dependancyInstallCommand" "$buildDir" || { echo "Failed to build dependancy, bailing out"; exit 1; }
}

function retrieveDependancy() {
    dependancyDir="$buildDir/lib"

    if ! [[ -d "$dependancyDir" ]]; then
        echo "$dependancyDir does NOT exist"
        createDirectory "$dependancyDir" || { echo "Failed to make dependancy lib directory, bailing out"; exit 1; }
    fi

    if ! [[ -d "$dependancyDir/$dependancyRootName" ]]; then
        changeDirectory "$dependancyDir"  || { echo "Failed to enter dependancy lib directory, bailing out"; exit 1; }
        runGitClone "$dependancySource" || { echo "Failed to retrieve dependancy source, bailing out"; exit 1; }
        changeDirectory "$dependancyDir/$dependancyRootName" || { echo "Failed to enter dependancy lib directory, bailing out"; exit 1; }
    else
        changeDirectory "$dependancyDir/$dependancyRootName"  || { echo "Failed to enter dependancy lib directory, bailing out"; exit 1; }
        runGitPullOrigin "$dependancySource" || { echo "Failed to retrieve dependancy source, bailing out"; exit 1; }
    fi
}

function generateDesktopFile() {
    copyFile "$utilityDir/$skeletonDesktopFileName" "$buildDir/$desktopFileName" || { echo "Failed to generate desktop file, bailing out"; exit 1; }
    copyFile "$iconPath" "$buildDir/" || { echo "Failed to generate desktop file, bailing out"; exit 1; }
    appendStringToFile "Exec=$buildDir$programName" "$buildDir/$desktopFileName" || { echo "Failed to generate desktop file, bailing out"; exit 1; }
    appendStringToFile "Icon=$buildDir$iconName" "$buildDir/$desktopFileName" || { echo "Failed to generate desktop file, bailing out"; exit 1; }
}


buildDir="build"
appDir="$HOME/.local/share/applications/"

trap bailout INT QUIT TERM
if [[ $# -gt 0 ]]; then
    if [[ "$1" == "--uninstall" ]]; then
        echo "Success"
        exit 0
    fi
    buildDir="$1"
    if ! [[ -d "$buildDir" ]]; then
        createDirectory "$buildDir" || { echo "Unable to create build directory \"$buildDir\", exiting"; exit 1; }
    fi
else
    buildDir="$filePath/$buildDir"
    if ! [[ -d "$buildDir" ]]; then
        createDirectory "$buildDir" || { echo "Unable to make build directory \"$buildDir\", exiting"; exit 1; }
    fi
fi

changeDirectory "$buildDir" || { echo "Unable to enter build directory \"$buildDir\""; exit 1; }
dependancyResult=$(ls "$libDir" | grep "$dependancyRootName")
if [[ -z "$dependancyResult" ]]; then
    doInternetCheck || { echo "There is no active internet connection, so dependancy cannot be retrieved, bailing out"; exit 1; }
    retrieveDependancy || { echo "Coult not retrieve dependancy source, bailing out"; exit 1; }
    buildDependancy || { echo "Building dependancy failed, bailing out"; exit 1; }
fi
doCygwinCheck
runQmake "$filePath" || { echo "qmake failed, bailing out"; exit 1; }
runMake || { echo "make failed, bailing out"; exit 1; }
generateDesktopFile || { echo "Could not generate desktop file, bailing out"; exit 1; }
copyFile "$buildDir/$desktopFileName" "$appDir" || { echo "Could not copy desktop file to application directory, bailing out"; exit 1; }
suLinkFile "$buildDir/$programName" "$globalBinDir"  || { echo "Could not link file, bailing out"; exit 1; }

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
