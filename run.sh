#!/bin/bash
#loading config, if exists; defaults if not.

function report_status(){
    echo Configuration: 
    echo dataset: $VXX_FILES
    echo surface: $VXX_SOURCE
    if [[ -z "$VXX_NO_FULLSCREEN" ]]; then echo fullscreen; fi;
};

function dump_status(){
DEST=config.sh
echo "VXX_NO_FULLSCREEN=$VXX_NO_FULLSCREEN" > $DEST
echo "VXX_SOURCE=$VXX_SOURCE" >> $DEST
echo "VXX_FILES=$VXX_FILES" >> $DEST
};

if [[ -f "config.sh" ]]; then 
    source config.sh; 
   else #defaults
    VXX_SOURCE=pial;
    VXX_FILES=data;
    VXX_NO_FULLSCREEN=1;
fi;
     
#checking commandline for file list.
if [[ -n "$@" ]]; then VXX_FILES="$@"; fi; 

report_status
#menu
select opt in go_go_go surface_type fullscreen_toggle; do 
case $opt in
    surface_type ) 
	echo enter surface type:
	read VXX_SOURCE;;
    fullscreen_toggle )
	if [[ -z "$VXX_NO_FULLSCREEN" ]]; then VXX_NO_FULLSCREEN=1; 
	else VXX_NO_FULLSCREEN=""; fi;;
    go_go_go )
	break;;
esac;
report_status;
done;

#Go-go-go!
dump_status;
echo ./vxx.t $VXX_FILES