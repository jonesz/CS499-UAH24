SRC=$PWD

cp -r $SRC/uah-os-pdclib/* ../ext/pdclib
cd ../ext/pdclib
_PDCLIB_BUILD=1 _PDCLIB_STATIC_DEFINE=1 cmake .
make -j4

mkdir -p $SRC/include
cp -r include/* $SRC/include
