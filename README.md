# Faster JPEG encode/decode processing

*OpenCV default JPEG compression is way too slow, we need one faster implement*


## Benchmark


* `restart_interval` matters, please tweak it for your specific image size. (8, 16, 32) can be used for `1280x720`. All test below use `16`.

Time on Xavier with `1280x720` images, *Note, the speed varies for different running, however, the speedup ratio is still satisfying.*

``` bash
./demo_benchmark.sh
```

C++ version:

| Method | Time ms (100 iterations) |
| :-----| :---- | 
| OpenCV Encode | 2369  |
| GPUJPEG Encode | 245  |
| OpenCV Decode | 1884  |
| GPUJPEG Decode | 429  |


Python version:

| Method | Time ms (100 iterations) |
| :-----| :---- | 
| OpenCV Encode | 791  |
| GPUJPEG Encode | 242  |
| OpenCV Decode | 1269  |
| GPUJPEG Decode | 492  |

## Usage

``` vi
# Build and install GPUJPEG first (CMake)
git clone https://github.com/CESNET/GPUJPEG.git
...

# Build this demo (CMake)
# Please change the OpenCV (and libgpujpeg) path
git clone https://github.com/MiaoDX/GPUJPEG_demo.git
...


./demo_benchmark.sh
```

## Optional choices

* [CESNET/GPUJPEG](https://github.com/CESNET/GPUJPEG.git) 
  - Current choice, open-source, quick enough
* [Fastvideo](https://www.fastcompression.com/)
  - Close-sourced SDK, many other functions
  - Watermark with trial version
* nvJPEG
  - Not available for Xavier currently (20191016)

## TODO

* Use OpenCV cv::cuda::GpuMat directly
