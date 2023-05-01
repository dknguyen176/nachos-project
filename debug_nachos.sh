#!/bin/bash
set -e

cd code/test
make

../build.linux/nachos -d u -x $*