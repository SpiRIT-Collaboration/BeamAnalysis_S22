# src_modified

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
