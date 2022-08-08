#!/bin/bash

mode="rand"
while read f ; do
  if grep "RAND" $f > /dev/null; then
     mode="rand"
  elif grep "nvm" $f > /dev/null; then
     mode="nvm"
  fi

  if "$mode" -eq "nvm" ; then
     echo "nvm" $f 
  else
     echo $f
  fi
done
