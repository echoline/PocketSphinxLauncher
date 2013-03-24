#!/bin/sh

HERE=$(dirname $0)
cd $HERE
HERE=$(pwd)

export PATH=$HERE/bin/:$PATH
export LD_LIBRARY_PATH=$HERE/lib/

PocketSphinxLauncher $@
