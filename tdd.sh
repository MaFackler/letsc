#!/bin/bash
ag -l | entr -c -r -s "make $1 && ./$1"
