#!/usr/bin/env python3
import sys

mode='rand'
#   with open('sys.stdin','r') as f:
for line in sys.stdin:
    line=line.strip()
    if 'RAND' in line :
        mode='rand'
    if 'nvm' in line :
        mode='nvm'
    if mode == 'nvm':
        print ('nvm', line)
    else:
        print ('dram',line)



    
