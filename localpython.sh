#!/bin/bash
# wget http://www.python.org/ftp/python/3.8.0/Python-3.8.0.tgz
# tar -zxvf Python-3.8.0.tgz
rm -rf .localpython
mkdir .localpython
cd Python-3.8.0
./configure --prefix=$PWD/../.localpython
make
make install
cd ..
