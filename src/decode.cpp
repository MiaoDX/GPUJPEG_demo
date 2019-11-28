/**
 * Fast GPU JPEG decoder example class with
 * [CESNET/GPUJPEG](https://github.com/CESNET/GPUJPEG)
 * compatible/call with OpenCV
 * DongxuMiao@DeepMotion.ai
 */

#include "decode.h"
#include "encode.h"

#include <opencv2/opencv.hpp>
// #include <opencv2/cudaimgproc.hpp>
#include <chrono>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: command <file> <restart_interval>!\n");
    return -1;
  }

  cv::Mat ocv_img = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  cv::cvtColor(ocv_img, ocv_img, cv::COLOR_BGR2RGB);

  // cv::imencode( ".jpg", ocv_img, encode_buf);  // only encode with
  // restart_interval can have potential speedup
//   FastEncoder(int width = 1280, int height = 720, int restart_interval = 16, int device_id = 0) {
  FastEncoder fastEncoder(ocv_img.size().width, ocv_img.size().height, atoi(argv[2]));
  std::vector<uint8_t> encode_buf = fastEncoder.encode(ocv_img);

  FastDecoder fastDecoder(ocv_img.size().width, ocv_img.size().height, atoi(argv[2]));

  int iterate = 100;

  cv::Mat im;

  auto start = std::chrono::steady_clock::now();
  for (int iteration = 0; iteration < iterate; iteration++) {
    im = fastDecoder.decode(encode_buf);
  }
  auto end = std::chrono::steady_clock::now();
  std::cout << "GPU decode time in milliseconds : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
  cv::imwrite("decode_gpu_cpp.jpg", im);

  start = std::chrono::steady_clock::now();
  cv::Mat encode_buf_mat(encode_buf);
  for (int iteration = 0; iteration < iterate; iteration++) {
    im = cv::imdecode(encode_buf_mat, CV_LOAD_IMAGE_COLOR);
  }
  end = std::chrono::steady_clock::now();
  std::cout << "GPU decode time in milliseconds : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
  cv::imwrite("decode_cv2_cpp.jpg", im);

  return 0;
}
