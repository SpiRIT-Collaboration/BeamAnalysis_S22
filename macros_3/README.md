# macros_3 #
Third step of the analysis, reconstruct the atomic number and mass to charge ratio of the beam isotope.

Same as **macros_1** and **macros_2**, macros here needs partly reconstructed data.
`RIDFtoROOT_S22.cc` generates root file with such information.


`RIDFtoROOT_S22.cc` : Generate root file with calibrated data and reconstructed beta and A/Q.

`anaBetaDifference.C` : Check how much difference there are when any focal plane tracking is not available.

`anaICcalibration.C` : Calibrate IonChamber by using un-calibrated IC data and reconstructed beta, A/Q.

`anaPPACefficiency.C` : Check PPAC efficiency for each focal plane by seeing the tracking data.

`anaRecoefficiency.C` : Check how the reconstruction efficiency is by seeing the A/Q value.

`anaTOF_Runbyrun.C` : See the TOF stability and estimate how much TOF offset is necessary.

`CombineTrees.cc` : Combine root files into one file with full information of beam PI, magnetic projection, and so on.

You have to open a path for the libraries of magnetic projection to execute CombineTrees file.
Please type `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../class` before using the CombineTrees.
