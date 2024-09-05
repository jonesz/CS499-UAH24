mkdir -p include/
cp -r ../ext/pdclib/include/* ./include

cd ../ext/pdclib
make
