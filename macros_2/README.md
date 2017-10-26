# macros_2 #

Second step of the analysis, analyze the raw data to obtain the calibration or correction parameters which will be added in database files.

Macros here need the root files with raw data set of detectors.
Same as the **macros_1** directory, you have to `make` and execute `run.sh`.
```bash
$ make
$ cd ../; mkdir -p rootfiles/rawdata; cd macros_2
$ source ./run.sh

$ root anaHOGEHOGE.C
```

`RIDFtoRAWROOT_S22.cc` : Generate root file with raw data container class of ANAROOT.

`anaBDCTimeWindow.C` : Obtain drift time histogram for the each layer of BDC.

`anaPPACTSumGate.C` : Obtain TSum gate parameter for each PPAC.

`anaPlasticSlewing.C` : Make ADC-TDC correlation histogram and get slewing correction parameter for plastics.

`ana[Detector]_Runbyrun.C` : See the detector stability for each run.
