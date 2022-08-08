#!/bin/bash

for SOCKET in 0 1; do
  for CPU in {0..31} ; do
    echo "socket $SOCKET cpu $CPU "
    ./mgen -a $SOCKET -c $CPU -t 5
  done
done
