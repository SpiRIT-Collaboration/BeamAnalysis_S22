# BeamAnalysis_S22
Instruction for macros and parameter files to analyze BigRIPS detectors of the S22 Experiment with ANAROOT.

## Description on the Directories in this Repository
**db/** : Parameter files for calibration and correction, 

**class/** : Class files to read TTree branches in generated ROOT files easier.

**macros_1/** : Macros to analyze Multi-hit TDC for PPACs and Plastics.

**macros_2/** : Macros to generate ROOT files of TTree with ANAROOT raw data container classes, and to calibrate or correct raw data using generated ROOT file.

**macros_3/** : Macros to reconstruct beta and A/Q using ANAROOT beam information classes, and to calibrate ion chamber energy deposit.

**macros_basic/** : Macros to check raw data from ridf data file. 

**runinfo/** : List of run mainly on useless data

**src_modified/** : Source codes which are modified from the default ones. (These are merged into [the SpiRIT ANAROOT](https://github.com/SpiRIT-Collaboration/anaroot/commit/f4b6ec282d46ba158a4019b8b8c7fd3daa34b8ff).)

**matrix/** : Matrices for each beam line magnet.

## Rough Analysis Flow
1. Decode the ridf-format files into TTree with raw data containers of TClonesArray.
1. Get the calibration or correction parameters by analyzing raw data.
1. Reconstruction Z, A/Q, beta ... etc.

So, you can follow the analysis by looking through each directory like, macros_1/ -> macros_2/ -> macros_3/.

### As a preparation...
Please setup the ANAROOT environment in your analysis server.
