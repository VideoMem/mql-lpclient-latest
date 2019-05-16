#!/bin/bash
cmake -DCMAKE_TOOLCHAIN_FILE=./toolchain-lnx-mingw32.cmake -DCMAKE_INSTALL_PREFIX=./build .
rm ./lpc/bin/liblpclient.dll
rm ./Libraries/liblpclient.dll
make clean
make
cp liblpclient.dll ./Libraries
cp liblpclient.dll ./lpc/bin/liblpclient.dll
cp ./lpc/bin/libzmq-* ./Libraries/
i686-w64-mingw32-g++ -c -o ./lpc/bin/lpclient_test.o ./lpc/lpclient_test.cpp
i686-w64-mingw32-g++ -static-libstdc++ -static-libgcc -o ./lpc/bin/lpclient_test.exe -s ./lpc/bin/lpclient_test.o -s ./lpc/bin/libzmq.dll -L. -llpclient
