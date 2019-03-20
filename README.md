# MC833 - Network Programming
Projects done to the subject MC833 - Network Programming

## Pre requisites

- [json-c](https://github.com/json-c/json-c)
- git

## Instalation

``` sh
$ sudo apt-get install git
$ sudo apt-get install cmake
$ sudo apt-get install autoconf automake libtool
```

### Json-c

``` sh
$ git clone https://github.com/json-c/json-c.git
$ cd json-c
$ sh autogen.sh
$ mkdir build
$ cd build
$ cmake ../ -DBUILD_SHARED_LIBS=OFF
$ make
$ cp libjson-c.a /path/to/project
```

## The project
```
$ make
```