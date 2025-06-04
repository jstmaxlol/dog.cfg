#!/bin/bash

read -p "are you struggling with the doxygen documentation? (y/n) " docsStruggle

if [[ "$docsStruggle" == "y" ]]; then
    cd ./html/ # $USER has to be in the root dir of the cloned repo to run this.. i hope T-T
    python -m http.server
    exit 0
elif [[ "$docsStruggle" == "Y" ]]; then
    cd ./html/ # $USER has to be in the root dir of the cloned repo to run this.. i hope T-T
    python -m http.server
    exit 0
else
    printf "unrecognized input"
    exit 1
fi 

exit
