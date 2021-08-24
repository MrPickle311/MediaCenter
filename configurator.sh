#!/usr/bin/bash

cleanupDir(){
    rm -rf $1 && mkdir $1
}

# it selects the latest Qt 5.* version
# @return /home/USER/Qt/5.* list
detectQtVersion(){
    declare -n arr="$1"
    while read -d $'\0' file ;do
        arr+=($file)
    done < <(find /home/$USER/Qt -maxdepth 1 -name "5.*" -print0)
}

# this function returns selected /home/USER/Qt/5.* via first argument
promptAboutQtVersion(){
    declare -a qt_versions
    declare -n selected_qt_dir_="$1"
    detectQtVersion qt_versions

    if [ ${#qt_versions[@]} -gt 1 ]; then
        echo "Multiple Qt5 versions detected:" 
        printf '%s\n' "${qt_versions[@]}"

        echo "Pass the desired Qt5 version ( SELECT INDEX , which starts from 0 ): "
        read selected_version
        selected_qt_dir_=${qt_versions[$selected_version]}
    else
        selected_qt_dir_=${qt_versions[0]}
    fi
}

#first arg is the selected Qt dir
#second is C++ compiler
runCmake(){
    declare cxx_compiler

    if [ -z "${2}" ]; then
        cxx_compiler="g++"
    else
        cxx_compiler="$2"
    fi

    cmake .. \
    -G"Unix Makefiles" \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_PROJECT_INCLUDE_BEFORE:PATH=/home/$USER/Qt/Tools/QtCreator/share/qtcreator/package-manager/auto-setup.cmake \
    -DQT_QMAKE_EXECUTABLE:STRING="${1}"/gcc_64/bin/qmake \
    -DCMAKE_PREFIX_PATH:STRING="${1}"/gcc_64 \
    -DCMAKE_C_COMPILER:STRING=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:STRING=/usr/bin/g++
}

#first is the configured dir
#second arg is the selected Qt dir
#third is C++ compiler
configureDir(){
    prev_path=$(pwd) # safe previous dir as backup
    cleanupDir $1 # clean previous build folder
    cd $1 # go to a certain build folder
    runCmake ${2} ${3} # configure 
    cd ${prev_path} # go back to the root folder
}

# first argument - build dir
buildDir(){
    prev_path=$(pwd) # safe previous dir as backup
    cd $1 # go to a certain build folder
    cmake --build .
    cd ${prev_path} # go back to the root folder
}

# first argument - tests dir
runTests(){
    prev_path=$(pwd) # safe previous dir as backup
    cd $1 # go to a certain build folder
    ctest
    cd ${prev_path} # go back to the root folder
}

declare selected_qt_dir
promptAboutQtVersion selected_qt_dir

#t - configure tests
#T - build tests

#c - configure project
#b - build project

#r - run tests

while getopts ":tTcbr" opt; do
  case ${opt} in
    t ) configureDir "tests/build" "${selected_qt_dir}"
      ;;
    T ) buildDir "tests/build"
      ;;
    c ) configureDir "build" "${selected_qt_dir}" 
      ;;
    b ) buildDir "build"
      ;;
    r ) runTests "tests/build"
      ;;
    \? ) echo "Usage: cmd [-b] [-t]"
      ;;
  esac
done
