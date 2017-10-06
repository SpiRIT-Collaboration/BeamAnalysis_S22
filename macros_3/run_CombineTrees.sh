#!/bin/sh


classDir=/home/s015/exp/exp1605_s015/anaroot/users/S22_analysis/class

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$classDir

cd ./supplemental/
source run_CombineTrees0.sh & source run_CombineTrees1.sh & source run_CombineTrees2.sh & source run_CombineTrees3.sh
cd ../
