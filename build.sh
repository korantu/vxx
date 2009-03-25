#!/bin/bash

echo Using `uname`...
make -f Makefile.`uname` $@
