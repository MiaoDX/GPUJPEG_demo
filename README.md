# Faster JPEG encode/decode processing

*OpenCV default JPEG compression/decompression is way too slow, we need one faster implement*

**`restart_interval` matters as [GPU parallel processing relaying on it, CESNET/GPUJPEG/issues/29](https://github.com/CESNET/GPUJPEG/issues/29). Please tweak it for specific image size. (8, 16, 32) can be used for `1280x720`.**

## Benchmark

Time with `1280x720` images and `restart_interval=16` on Xavier and Allienware(i7-7820HK@2.90GHz×8, GeForce GTX 1080 with Max-Q). *Note, the speed varies for different running, however, the speedup ratio is still satisfying.*

``` bash
./demo_benchmark.sh
```

**Time ms (100 iterations)**

C++ version:

| Method | Xavier | GTX1080(Max-Q) |
| :-----| :---- | :---- |
| OpenCV Encode | 2369  | 739 |
| GPUJPEG Encode | 245  | 152 |
| OpenCV Decode | 1884  | 829 |
| GPUJPEG Decode | 429  | 206 |


Python version:

| Method |  Xavier | GTX1080(Max-Q) |
| :-----| :---- |  :---- |
| OpenCV Encode | 791  | 704 |
| GPUJPEG Encode | 242  | 154 |
| OpenCV Decode | 1269  | 1092 |
| GPUJPEG Decode | 492  | 263 |

## Usage

``` vi
# Build and install GPUJPEG first (CMake)
git clone https://github.com/CESNET/GPUJPEG.git
...

# Build this demo (CMake)
# Please change the OpenCV (and libgpujpeg) path
git clone https://github.com/MiaoDX/GPUJPEG_demo.git --depth 1
git submodule update --init --depth 1
...

./demo_benchmark.sh
```

## Optional choices

* [CESNET/GPUJPEG](https://github.com/CESNET/GPUJPEG.git) 
  - Current choice, open-source, quick enough
* [Fastvideo](https://www.fastcompression.com/)
  - Close-sourced SDK, many other functions
  - Watermark with the trial version
* nvJPEG
  - Not available for Xavier currently (20191016)

## Changelog

* 2019.11.28, `decode`, python binding, and benchmark
* 2019.11.27, refactoring and python binding for `encode` with pybind
* 2019.10.16, initial version, example codes for `encode`

## TODO

* Use OpenCV `cv::cuda::GpuMat` directly
