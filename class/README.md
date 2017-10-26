# class

Class files which enable us to read TTree with ANAROOT based TBranch.

It's easy if you make the shared library on the ROOT and read it in the analysis.

```bash
$ root
[] .L BeamRaw.C++ (or .L BeamReco.C++)
[] .q
```
And edit rootlogon.C file in your analysis directory so that ROOT can recognize the shared library.
```vim
void rootlogon(){

  gSystem->Load("DIRECTORY/S22_analysis/class/BeamRaw_C.so");

}
```

Makefiles are for the classes on the beam transport toward the target(magnetic projection, energy deposit).
They are used in the `macros_3/CombineTrees.cc` code.
