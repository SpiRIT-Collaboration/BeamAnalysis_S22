#class#

Class files which enable us to read TTree with ANAROOT based TBranch.



```bash
$ root
[] .L BeamRaw.C++ (or .L BeamReco.C++)
[] .q

After generating the shared library, add the rootlogon.C file as "gSystem->Load("DIRECTORY/S22_analysis/class/BeamRaw_C.so");"
```

Makefiles are for the classes on the beam transport toward the target(magnetic projection, energy deposit).
They are used in the `macros_3/CombineTrees.cc` code.
