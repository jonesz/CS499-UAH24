In the root of the repo.

```
mkdir cross-prebuilt
cd cross-prebuilt
wget https://mirrors.edge.kernel.org/pub/tools/crosstool/files/bin/x86_64/14.2.0/x86_64-gcc-14.2.0-nolibc-i386-linux.tar.xz
tar xvf x86_64-gcc-14.2.0-nolibc-i386-linux.tar.xz
ln gcc-14.2.0-nolibc/i386-linux/bin/i386-linux-gcc gcc-14.2.0-nolibc/i386-linux/bin/i686-elf-gcc
ln gcc-14.2.0-nolibc/i386-linux/bin/i386-linux-ar gcc-14.2.0-nolibc/i386-linux/bin/i686-elf-ar
cd ../
rm -rf ext/*
git submodule update --init --recursive
```

You'll run this command to insert the compiler binaries into your path each time you want to do development; run this in the root of the repo.

```
export PATH=$PATH:$PWD/cross-prebuilt/gcc-14.2.0-nolibc/i386-linux/bin
```

You can then

```
cd src/
make
qemu-system-i386 kernel.iso
```
