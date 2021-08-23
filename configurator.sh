#!/bin/bash

cleanupDir(){
    rm -rfv $1 && mkdir $1
}

configureDir(){
    cmake \
    -S /home/damiano/Projects/MediaCenter/tests \
    -B /home/damiano/Projects/MediaCenter/tests/build \
    -G"Unix Makefiles" \
    -DCMAKE_BUILD_TYPE:STRING=Debug \
    -DCMAKE_PROJECT_INCLUDE_BEFORE:PATH=/home/damiano/Qt/Tools/QtCreator/share/qtcreator/package-manager/auto-setup.cmake \
    -DQT_QMAKE_EXECUTABLE:STRING=/home/damiano/Qt/5.15.2/gcc_64/bin/qmake \
    -DCMAKE_PREFIX_PATH:STRING=/home/damiano/Qt/5.15.2/gcc_64 \
    -DCMAKE_C_COMPILER:STRING=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER:STRING=/usr/bin/g++-10
}

cleanupDir "build"
cleanupDir "tests/build"

cmake \
-S /home/damiano/Projects/MediaCenter/tests \
-B /home/damiano/Projects/MediaCenter/tests/build \
-G"Unix Makefiles" \
-DCMAKE_BUILD_TYPE:STRING=Debug \
-DCMAKE_PROJECT_INCLUDE_BEFORE:PATH=/home/damiano/Qt/Tools/QtCreator/share/qtcreator/package-manager/auto-setup.cmake \
-DQT_QMAKE_EXECUTABLE:STRING=/home/damiano/Qt/5.15.2/gcc_64/bin/qmake \
-DCMAKE_PREFIX_PATH:STRING=/home/damiano/Qt/5.15.2/gcc_64 -DCMAKE_C_COMPILER:STRING=/usr/bin/gcc -DCMAKE_CXX_COMPILER:STRING=/usr/bin/g++-10