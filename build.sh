#!/bin/bash

echo Using `uname -o`...
case `uname -o` in
Cygwin ) make -f Makefile.platform.Cygwin $@ ;;
*Linux ) make -f Makefile.platform.Linux $@ ;;
* ) echo "No custom makefile for the platform";;
esac
