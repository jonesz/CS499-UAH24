SRC=$PWD

cp Makefile.PDCLIB ../ext/pdclib/Makefile
cp -r $SRC/uah-os-pdclib/* ../ext/pdclib
cd ../ext/pdclib
make
cp libk.a $SRC/

mkdir -p $SRC/include
cp -r include/ $SRC/include/libc
