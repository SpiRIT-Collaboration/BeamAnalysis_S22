#!/bin/sh

nodatafile=../../runinfo/nodatalist.txt
noplastic=../../runinfo/nodataplastic.txt
noppac=../../runinfo/nodatappac.txt
noic=../../runinfo/nodataic.txt

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
  
  noplaflag=0
  for noplanum in `cat "$noplastic"`
    do
    if [ $number -eq $noplanum ]; then
      noplaflag=1
    fi
  done

  noicflag=0
  for noicnum in `cat "$noic"`
    do
    if [ $number -eq $noicnum ]; then
      noicflag=1
    fi
  done

  noppacflag=0
  for noppacnum in `cat "$noppac"`
    do
    if [ $number -eq $noppacnum ]; then
      noppacflag=1
    fi
  done


  if [ $nodataflag -eq 0 -a $noplaflag -eq 0 -a $noicflag -eq 0 -a $noppacflag -eq 0 ]; then
    if [ $number -ge 2431 -a $number -le 2509 ] || [ $number -ge 2520 -a $number -le 2570 ]; then
      cd ..
      ./CombineTrees $number || { echo ROOTerror!; exit 1 ;}
      echo "RUN$number finish."
      cd ./supplemental
    fi
  fi
  done
