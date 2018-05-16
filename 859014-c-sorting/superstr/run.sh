#!/bin/bash

if test -z $1
then
    n=1
else
    n=$1
fi

cat input$n.txt | ./superstr

