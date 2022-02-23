# CDRTools + DWDRWTools

* The Cdrtools package contains CD recording utilities. These are useful for reading, creating or writing (burning) Compact Discs.  

* The dvd+rw-tools package contains several utilities to master the DVD media, both +RW/+R and -R[W]. The principle tool is growisofs which provides a way to both lay down and grow an ISO9660 file system on (as well as to burn an arbitrary pre-mastered image to) all supported DVD media. This is useful for creating a new DVD or adding to an existing image on a partially burned DVD.
* This package is known to build and work properly using an LFS-11.0 platform.

# CDRecord changes

* This repository contains code from original sources and patches created by Acronis for cdrecord.
* The changes in this repository are to support Windows and Mac platforms and to enable building a 64-bit client.

## Alternate reference

<https://www.linuxfromscratch.org/blfs/view/6.3/multimedia/cdrtools.html>

## Sources

### CDRTools 2.01

<ftp://ftp.berlios.de/pub/cdrecord/cdrtools-2.01.tar.bz2>
<http://gd.tuwien.ac.at/utils/schilling/cdrtools/cdrtools-2.01.tar.bz2>

MD5 sum: d44a81460e97ae02931c31188fe8d3fd

### dvd+rw-tools 7.1

<http://fy.chalmers.se/~appro/linux/DVD+RW/tools/dvd+rw-tools-7.1.tar.gz>

MD5 sum: 8acb3c885c87f6838704a0025e435871


## Acronis Patches

./packages/cdrtools/2.01/bootcd.ru-src.patch
./packages/cdrtools/2.01/cdrecord-64bit_compile.patch
./packages/cdrtools/2.01/cdrecord-abort.patch
./packages/cdrtools/2.01/cdrecord-aspi.patch
./packages/cdrtools/2.01/cdrecord-author.patch
./packages/cdrtools/2.01/cdrecord-batchmode.patch
./packages/cdrtools/2.01/cdrecord-eject.patch
./packages/cdrtools/2.01/cdrecord-fixate.patch
./packages/cdrtools/2.01/cdrecord-libschily.patch
./packages/cdrtools/2.01/cdrecord-maxwait.patch
./packages/cdrtools/2.01/cdrecord-mingw.patch
./packages/cdrtools/2.01/cdrecord-novalloc.patch
./packages/cdrtools/2.01/cdrecord-readcd.patch
./packages/cdrtools/2.01/cdrecord-scsi-mac.patch
./packages/cdrtools/2.01/cdrecord-zerospeed.patch

./packages/dvd+rw-tools/7.1/growisofs.patch



## Windows i686

### Msys

Download installer from: <http://www.mingw.org/wiki/msys>

Install following packages:

* mingw32-base
* mingw32-gcc-g++
* msys-base
* msys-patch
* msys-m4

### Python 3.7

Download from: <https://www.python.org/downloads/release/python-374/>

#### OpenWatcom 1.9.0 _(used to embed Windows resources)_

Download from: <http://ftp.openwatcom.org/install/>
Modify environment and let installer set WATCOM env. variable

#### Environment

``` batch
set PATH=C:\Python27;C:\Python27\Scripts;C:\MinGW\bin;C:\MinGW\msys\1.0\bin;C:\Windows\System32
```

## Linux i686/x86_64

build in a Docker container with lx/lxa64 compiler




