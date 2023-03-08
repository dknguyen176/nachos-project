#!/bin/bash
set -e

cd code/build.linux
make distclean

cd ../../coff2noff
make distclean

cd ../code/test
make distclean