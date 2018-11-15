# README

Open Source Software for Developing Secure Applications on the Freedom E Platform

Hex Five MultiZone Security submodule for inclusion in SiFive Freedom E SDK

This repository, maintained by Hex Five Security, makes it easy to build robust Trusted Execution Environments on SiFive's Freedom E RISC-V platform. 

### Installation ###

prerequisites:

- https://github.com/sifive/freedom-e-sdk (see relative install notes)
- java jre 1.8 

```
git clone https://github.com/hex-five/multizone-freedom-e-sdk.git
```

rename folder `multizone-freedom-e-sdk` to `multizone_security`

move folder `multizone_security` to `~/freedom-e-sdk/software/multizone_security`


### Usage ###

```
cd ~/freedom-e-sdk
```

- to clean: `make clean PROGRAM=multizone_security BOARD=coreplexip-e31-arty`

- to build: `make software PROGRAM=multizone_security BOARD=coreplexip-e31-arty`

- to upload & run: `make upload PROGRAM=multizone_security BOARD=coreplexip-e31-arty`

- to debug: open two terminal sessions:

   session 1: `make run_openocd BOARD=coreplexip-e31-arty`

   session 2: `make run_gdb PROGRAM=multizone_security BOARD=coreplexip-e31-arty`
```
           (gdb) add-symbol-file ./software/multizone_security/zone1/zone1.elf 0x40410000
           (gdb) break main
           (gdb) info local
           (gdb) ctrl-c
           (gdb) continue 
```

### Note ###

Supported boards:

- coreplexip-e31-arty (rv32)
- coreplexip-e51-arty (rv64)

- to debug zone2: `(gdb) add-symbol-file ./software/multizone_security/zone2/zone2.elf 0x40420000`
- to debug zone3: `(gdb) add-symbol-file ./software/multizone_security/zone3/zone3.elf 0x40430000`

Ubuntu 18.04:

The prebuilt toolchain provided by SiFive doesn't work with Ubuntu 18.04. You can either build the one included in the repo (approx 20 minutes build time) or point to the one packaged with FreedomStudio. Then make sure the environment points to these folders:

```
export RISCV_PATH=/home/hexfive/riscv64-unknown-elf-gcc-20180928-x86_64-linux-centos6
export RISCV_OPENOCD_PATH=/home/hexfive/riscv-openocd-20180928-x86_64-linux-centos6
```

If java is not installed in your system:

```
sudo apt install openjdk-8-jre-headless
```

### For More Information ###

See [MultiZone_Manual](https://github.com/hex-five/multizone-freedom-e-sdk/blob/master/multizone_sdk_manual.pdf)

Documentation, forums, and more examples of prebuilt secure implementations of common open source stack components:

[hex-five.com](https://hex-five.com)
