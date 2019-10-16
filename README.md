# Demo usage of [CESNET/GPUJPEG](https://github.com/CESNET/GPUJPEG)

``` vi
# Build and install GPUJPEG first (cmake way)
git clone https://github.com/CESNET/GPUJPEG.git
...


# Build this demo (cmake way)
# Please change the OpenCV (and libgpujpeg) pathes
git clone https://github.com/MiaoDX/GPUJPEG_demo.git
...


# Run with images
./build/cuda_jpeg_encode images/a.png
./build/cuda_jpeg_encode images/b.jpg
```

## TODO

* Use OpenCV cv::cuda::GpuMat directly
