#!/bin/bash
set -e

cd code/test
make

../build.linux/nachos -s -d + -x $*