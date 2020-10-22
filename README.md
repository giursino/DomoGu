# DomoGu

## A simple building automation application
[![Build Status](https://travis-ci.com/giursino/DomoGu.svg?branch=master)](https://travis-ci.org/giursino/DomoGu)

### Building
The project require a working C++11 compiler
```Shell
mkdir build
cd build 
cmake ..
cmake --build .
```

#### Building with test support
If you want to install CppUtest on System Environment please use distribution package, ex: `sudo apt-get install libcpputest-dev` and then follow building section setting ENABLE_TEST=ON on cmake invocation.
Else download latest CppUtest source and install to your local directory and call cmake setting variable CPPUTEST_PATH. Please, follow this example:
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
```



### Installing
Follow the build steps and simply execute
```Shell
sudo make install
```
