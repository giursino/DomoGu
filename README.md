# DomoGu

## A simple home automation application
[![Build Status](https://travis-ci.org/giursino/DomoGu.svg?branch=master)](https://travis-ci.org/giursino/DomoGu)

### For users

#### Requires
* C++11 (GCC 6.3)
* CMAKE >= 3.5
* pkgconfig
* [knxtinyserial](https://github.com/giursino/knxtinyserial)

#### Compiling and building
The project require a working C++11 compiler
```Shell
mkdir build
cd build 
cmake ..
cmake --build .
```

#### Installing
Follow the build steps and simply execute
```Shell
sudo make install
```
### For developers

#### Building with test support
If you want to install [CppUtest](https://cpputest.github.io) on System Environment please use distribution package, ex: `sudo apt-get install libcpputest-dev` and then follow building section setting ENABLE_TEST=ON on cmake invocation.
Please, follow this example:
```Shell
# build project with test support
cd <project dir>
mkdir build
cd build 
cmake -DENABLE_TEST=ON ..
cmake --build .
ctest
```

Else download latest [CppUtest](https://cpputest.github.io) source and install to your local directory and call cmake setting variable CPPUTEST_PATH. 
Please, follow this example:
```Shell
# download and install CppUTest
wget https://github.com/cpputest/cpputest/releases/download/v3.8/cpputest-3.8.tar.gz -O /tmp/cpputest.tar.gz
tar -xzvf /tmp/cpputest.tar.gz -C $HOME
cd $HOME/cpputest-* && ./configure --prefix=$HOME/cpputest-build && make && make install && cd -

# build project with test support
cd <project dir>
mkdir build
cd build 
cmake -DENABLE_TEST=ON -DCPPUTEST_PATH=$HOME/cpputest-build ..
cmake --build .
ctest
```

#### Build distribution
```
# build a binary distribution
cpack --config CPackConfig.cmake

# build a source distribution
cpack --config CPackSourceConfig.cmake
```
