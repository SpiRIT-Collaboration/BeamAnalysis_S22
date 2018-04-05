# src_modified

**Note: These are merged into [the SpiRIT ANAROOT](https://github.com/SpiRIT-Collaboration/anaroot/commit/f4b6ec282d46ba158a4019b8b8c7fd3daa34b8ff).**

These are the modified source code which was used in the S022 beam analysis.

If you want to use them to decode the raw data,
```bash
cp *.cc ANAROOT_SOURCE_DIRECTORY/sources/Reconstruction/BigRIPS/src/
cp *.hh ANAROOT_SOURCE_DIRECTORY/sources/Reconstruction/BigRIPS/include/

cd ANAROOT_SOURCE_DIRECTORY
make clean
make distclean
./autogen.sh --prefix=$PWD
make install
```
