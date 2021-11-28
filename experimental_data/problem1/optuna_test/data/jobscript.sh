#!/bin/bash

dir=$(cd $(dirname $0); pwd)

(cd $dir; ./a.out $1 $2 $3 $4 $5 $6 $7)
