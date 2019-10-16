# Faster JPEG decode processing

*OpenCV default JPEG compression is way too slow, we need one faster implement*

Time on Xavier with `1280x720` images

| Method | Time ms (100 iterations) |
| :-----| :---- | 
| OpenCV | 2411  |
| GPUJPEG | 160  |

## Usage

``` vi
# Build and install GPUJPEG first (CMake)
git clone https://github.com/CESNET/GPUJPEG.git
...

# Build this demo (CMake)
# Please change the OpenCV (and libgpujpeg) path
git clone https://github.com/MiaoDX/GPUJPEG_demo.git
...

# Run with images
./build/cuda_jpeg_encode images/a.png
./build/cuda_jpeg_encode images/b.jpg
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
