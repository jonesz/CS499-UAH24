# Cross Compiler Setup

[OSDev GCC Cross Compiler](https://wiki.osdev.org/GCC_Cross-Compiler)

Install the dependencies listed on this page; reading through it is wise. Below is what I've done in
an attempt to build the cross-compiler.

Install NASM:

`apt-get install nasm`

Fetch source for binutils/gcc:

```
wget https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.gz
wget https://ftp.gnu.org/gnu/binutils/binutils-2.43.tar.gz

tar xvf gcc-14.2.0.tar.gz
tar xvf binutils-2.43.tar.gz
```

Setup build environment:

```
export PREFIX="$PWD"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
```

Build binutils:

```
mkdir build-binutils
cd build-binutils
../binutils-2.43/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install
```

Build gcc:

```
which -- $TARGET-as || echo $TARGET-as is not in the PATH

mkdir build-gcc
cd build-gcc
../gcc-14.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
```

Install QEMU:

```
  sudo apt-get install qemu-system-x86
```

Running the kernel.

```
 cd src/
 make
 qemu-system-i386 -kernel kernel.bin
```
