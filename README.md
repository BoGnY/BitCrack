# BitCrack

A tool for brute-forcing Bitcoin private keys. The main purpose of this project is to contribute to the effort of solving the [Bitcoin puzzle transaction](https://blockchain.info/tx/08389f34c98c606322740c0be6a7125d9860bb8d5cb182c02f98461e5fa6cd15): A transaction with 32 addresses that become increasingly difficult to crack.

# Modification

> Contain tweaks from the following forks: ByLamacq, Radrigo, frstrtr (not implemented in .exe; you'll have to rename main_alt to main and compile), L0laapk3, and any others that have made apparent improvements to the original by brichard19. All of you are wizards and I bow to your superior abilities!
>
> This code also incorporates the popular but now removed pikachunakapika fork containing random mode along with the other tweaks. I found the source in the closed pull request in the original brichard19 repo. https://github.com/brichard19/BitCrack/pull/148/files

### Using BitCrack

#### Usage

Use `cuBitCrack.exe` for CUDA devices and `clBitCrack.exe` for OpenCL devices.

### Note: **clBitCrack.exe is still EXPERIMENTAL**, as users have reported critical bugs when running on some AMD and Intel devices.

**Note for Intel users:**

There is bug in Intel's OpenCL implementation which affects BitCrack. Details here: https://github.com/brichard19/BitCrack/issues/123

```
xxBitCrack.exe [OPTIONS] [TARGETS]

Where [TARGETS] are one or more Bitcoin address

Options:

-i, --in FILE
    Read addresses from FILE, one address per line. If FILE is "-" then stdin is read

-o, --out FILE
    Append private keys to FILE, one per line

-d, --device N
    Use device with ID equal to N

-b, --blocks BLOCKS
    The number of CUDA blocks

-t, --threads THREADS
    Threads per block

-p, --points NUMBER
    Each thread will process NUMBER keys at a time

-r, --random
    Each point will start in random KEYSPACE

--keyspace KEYSPACE
    Specify the range of keys to search, where KEYSPACE is in the format,

	START:END start at key START, end at key END
	START:+COUNT start at key START and end at key START + COUNT
    :END start at key 1 and end at key END
	:+COUNT start at key 1 and end at key 1 + COUNT

-c, --compressed
    Search for compressed keys (default). Can be used with -u to also search uncompressed keys

-u, --uncompressed
    Search for uncompressed keys, can be used with -c to search compressed keys

--compression MODE
    Specify the compression mode, where MODE is 'compressed' or 'uncompressed' or 'both'

--list-devices
    List available devices

--stride NUMBER
    Increment by NUMBER

--share M/N
    Divide the keyspace into N equal sized shares, process the Mth share

--continue FILE
    Save/load progress from FILE
```

#### Examples

The simplest usage, the keyspace will begin at 0, and the CUDA parameters will be chosen automatically
```
xxBitCrack.exe 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH
```

Multiple keys can be searched at once with minimal impact to performance. Provide the keys on the command line, or in a file with one address per line
```
xxBitCrack.exe 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH 15JhYXn6Mx3oF4Y7PcTAv2wVVAuCFFQNiP 19EEC52krRUK1RkUAEZmQdjTyHT7Gp1TYT
```

To start the search at a specific private key, use the `--keyspace` option:

```
xxBitCrack.exe --keyspace 766519C977831678F0000000000 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH
```

The `--keyspace` option can also be used to search a specific range:

```
xxBitCrack.exe --keyspace 80000000:ffffffff 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH
```

To periodically save progress, the `--continue` option can be used. This is useful for recovering
after an unexpected interruption:

```
xxBitCrack.exe --keyspace 80000000:ffffffff 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH
...
GeForce GT 640   224/1024MB | 1 target 10.33 MKey/s (1,244,659,712 total) [00:01:58]
^C
xxBitCrack.exe --keyspace 80000000:ffffffff 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH
...
GeForce GT 640   224/1024MB | 1 target 10.33 MKey/s (1,357,905,920 total) [00:02:12]
```

Use the `-b,` `-t` and `-p` options to specify the number of blocks, threads per block, and keys per thread.
```
xxBitCrack.exe -b 32 -t 256 -p 16 1FshYsUh3mqgsG29XpZ23eLjWV8Ur3VwH
```

### Choosing the right parameters for your device

GPUs have many cores. Work for the cores is divided into blocks. Each block contains threads.

There are 3 parameters that affect performance: blocks, threads per block, and keys per thread.

`blocks:` Should be a multiple of the number of compute units on the device. The default is 32.

`threads:` The number of threads in a block. This must be a multiple of 32. The default is 256.

`Keys per thread:` The number of keys each thread will process. The performance (keys per second)
increases asymptotically with this value. The default is 256. Increasing this value will cause the
kernel to run longer, but more keys will be processed.

### Build dependencies

For **CUDA**: CUDA Toolkit 10.2 [(info about NVIDIA architecture names)](https://arnon.dk/matching-sm-architectures-arch-and-gencode-for-various-nvidia-cards/)

For **OpenCL**: An OpenCL SDK (The CUDA toolkit contains an OpenCL SDK), or [ROCm Radeon Open Compute](https://github.com/RadeonOpenCompute/ROCm).

##### Windows build dependencies

Visual Studio 2019 with C/C++ packages

##### Linux build dependencies

Install dependencies with:

```bash
sudo apt install make gcc-8 g++-8
```

If you have multiple gcc/g++ version (and more recent, ex. 9 or 10) when you build CUDA project it is possible you get the following error: _unsupported GNU version! gcc versions later than 8 are not supported!_

To solve problem, do this trick (thanks to this [StackOverflow question](https://stackoverflow.com/questions/6622454/cuda-incompatible-with-my-gcc-version)) that change gcc/g++ compilator only for CUDA's nvcc builds, otherwise change system default gcc/g++ compilator using `update-alternatives`:

```bash
sudo ln -s /usr/bin/gcc-8 /usr/local/cuda/bin/gcc
sudo ln -s /usr/bin/g++-8 /usr/local/cuda/bin/g++
```

##### Docker build dependencies

Required Docker v19.03.0+ and Docker Compose v1.27.0+.

**NOTE:** More info about CUDA-GCC compatibility [here](https://gist.github.com/ax3l/9489132).

### Building in Windows

Open the Visual Studio solution.

Build the `clKeyFinder` project for an OpenCL build.

Build the `cuKeyFinder` project for a CUDA build.

Note: By default the NVIDIA OpenCL headers are used. You can set the header and library path for
OpenCL in the `BitCrack.props` property sheet.

### Building in Linux

Using `make`:

Build CUDA:
```bash
make BUILD_CUDA=1
```

Build OpenCL:
```bash
make BUILD_OPENCL=1
```

Or build both:
```bash
make BUILD_CUDA=1 BUILD_OPENCL=1
```

### Building in Docker

Using `make`:

Build CUDA:
```bash
docker-compose up cuda
```

Build OpenCL:
```bash
docker-compose up opencl
```

Or build both:
```bash
docker-compose up cuda opencl
```

### Supporting this project

If you find this project useful and would like to support it, consider making a donation. Your support is greatly appreciated!

##### brichard19's donation addresses:

**BTC**: `1LqJ9cHPKxPXDRia4tteTJdLXnisnfHsof`

**LTC**: `LfwqkJY7YDYQWqgR26cg2T1F38YyojD67J`

**ETH**: `0xd28082CD48E1B279425346E8f6C651C45A9023c5`

### Contact

Send any questions or comments to bitcrack.project@gmail.com
