/**
 * Fast GPU JPEG decoder example class with
 [CESNET/GPUJPEG](https://github.com/CESNET/GPUJPEG)
 * compatible/call with OpenCV
 * DongxuMiao@DeepMotion.ai

 * https://github.com/CESNET/GPUJPEG/issues/29 data to decode should have
 restart inside, otherwise, it will not be too quick
 */

#include <libgpujpeg/gpujpeg.h>
#include <libgpujpeg/gpujpeg_util.h>

#include <opencv2/opencv.hpp>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class FastDecoder {
 public:
  cv::Mat decode(std::vector<uint8_t>& buf) {
    if (gpujpeg_decoder_decode(decoder, buf.data(), buf.size(), &decoder_output) != 0) {
      fprintf(stderr, "Decode buf error\n");
    }
    cv::Mat im(param_image.height, param_image.width, CV_8UC3, decoder_output.data);
    return im;
  }

  py::bytes decode_npy(char* buf, int buf_len) {
    if (gpujpeg_decoder_decode(decoder, (uint8_t*)buf, buf_len, &decoder_output) != 0) {
      fprintf(stderr, "Decode buf error\n");
    }
    return py::bytes((const char*)decoder_output.data, decoder_output.data_size);
  }

  FastDecoder(int width = 1280, int height = 720, int restart_interval = 16, int device_id = 0) {
    fprintf(stdout, "GPU encoder init");
    int flags = GPUJPEG_VERBOSE;
    if (gpujpeg_init_device(device_id, flags) != 0) {
      fprintf(stderr, "Init on gpu device error\n");
    }

    decoder = gpujpeg_decoder_create(NULL);
    if (decoder == NULL) {
      fprintf(stderr, "Create decode error\n");
    }

    gpujpeg_set_default_parameters(&param);
    param.restart_interval = restart_interval;
    param.interleaved = 1;

    gpujpeg_image_set_default_parameters(&param_image);
    param_image.width = width;
    param_image.height = height;
    param_image.comp_count = 3;

    gpujpeg_decoder_set_output_format(decoder, GPUJPEG_RGB, GPUJPEG_444_U8_P012);
    // or eg. GPUJPEG_YCBCR_JPEG and GPUJPEG_422_U8_P1020
    gpujpeg_decoder_output_set_default(&decoder_output);
  }

  ~FastDecoder() { gpujpeg_decoder_destroy(decoder); }

 private:
  struct gpujpeg_decoder* decoder = NULL;
  struct gpujpeg_parameters param;
  struct gpujpeg_image_parameters param_image;
  struct gpujpeg_decoder_output decoder_output;
};
