#!/bin/sh

nodatafile=../runinfo/nodatalist.txt

dir=/home/s015/ridf/sdaq02/

for file in `ls $dir | egrep 'SMDAQ[0-9]{4}.ridf'`
  do
  number=${file%.*}
  number=${number#SMDAQ}
  nodataflag=0
  for nodatanum in `cat "$nodatafile"`
    do
    if [ $number -eq $nodatanum ]; then 
      nodataflag=1
    fi
    done
  
  if [ $nodataflag -eq 0 ]; then
    if [ $number -ge 2174 -a $number -le 2653 ]; then
    ./RIDFtoPPACRAW $number || { echo ROOTerror!; exit 1 ;}
    ./RIDFtoPLARAW $number || { echo ROOTerror!; exit 1 ;}
    echo "RUN$number finish."
    fi
  fi
done
  
