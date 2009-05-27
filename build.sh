#!/bin/bash

echo Using `uname -o`...
make -f Makefile.platform.`uname -o` $@
