/*
* GPUJPEG encode example, with OpenCV
*/

#include "encode.h"
// #include <opencv2/cudaimgproc.hpp>
#include <chrono>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: command <file> <restart_interval>!\n");
    return -1;
  }

  cv::Mat ocv_img = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
  cv::cvtColor(ocv_img, ocv_img, cv::COLOR_BGR2RGB);
  // cv::cuda::GpuMat ocv_img_gpu;
  // ocv_img_gpu.upload(ocv_img);

//   FastEncoder(int width = 1280, int height = 720, int restart_interval = 16, int device_id = 0) {
  FastEncoder fastEncoder(ocv_img.size().width, ocv_img.size().height, atoi(argv[2]));

  int iterate = 100;

  auto start = std::chrono::steady_clock::now();
  std::vector<uint8_t> buf;
  for (int iteration = 0; iteration < iterate; iteration++) {
    buf = fastEncoder.encode(ocv_img);
  }
  auto end = std::chrono::steady_clock::now();
  std::cout << "GPU encode time in milliseconds : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
  cv::Mat img_decode = cv::imdecode(cv::Mat(buf), CV_LOAD_IMAGE_COLOR);
  cv::imwrite("encode_gpu_cpp.jpg", img_decode);

  start = std::chrono::steady_clock::now();
  std::vector<uint8_t> buf_ocv;
  for (int iteration = 0; iteration < iterate; iteration++) {
    cv::imencode(".jpg", ocv_img, buf_ocv);
  }
  end = std::chrono::steady_clock::now();
  std::cout << "CV2 encode time in milliseconds : "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms" << std::endl;
  img_decode = cv::imdecode(cv::Mat(buf), CV_LOAD_IMAGE_COLOR);
  cv::imwrite("encode_cv2_cpp.jpg", img_decode);

  return 0;
}
