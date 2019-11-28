/*
* GPUJPEG encode example, with OpenCV
*/

#include "encode.h"
// #include <opencv2/cudaimgproc.hpp> 
#include <chrono>

int
main(int argc, char *argv[])
{

    if ( argc < 2 ) {
        fprintf(stderr, "Please supply source image filename!\n");
        return -1;

    }

    cv::Mat ocv_img = cv::imread( argv[1], cv::IMREAD_UNCHANGED );
    cv::cvtColor(ocv_img, ocv_img, cv::COLOR_BGR2RGB);
    // cv::cuda::GpuMat ocv_img_gpu;
    // ocv_img_gpu.upload(ocv_img);

    FastEncoder fastEncoder;

    int iterate = 100;


    auto start = std::chrono::steady_clock::now();
    std::vector<uint8_t> buf;
    for ( int iteration = 0; iteration < iterate; iteration++ ) {
        // fastEncoder.encode(ocv_img, buf);
        fastEncoder.encode(ocv_img.data, buf);
    }   
    auto end = std::chrono::steady_clock::now();
    std::cout << "GPU encode time in milliseconds : " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;
    cv::Mat img_decode=cv::imdecode(cv::Mat(buf), CV_LOAD_IMAGE_COLOR);
    cv::imwrite("save_fast_result.jpg", img_decode);

    start = std::chrono::steady_clock::now();
    std::vector<uint8_t> buf_ocv;
    for ( int iteration = 0; iteration < iterate; iteration++ ) {
        cv::imencode( ".jpg", ocv_img, buf_ocv);
    }   
    end = std::chrono::steady_clock::now();
    std::cout << "CV2 encode time in milliseconds : " 
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;
    img_decode=cv::imdecode(cv::Mat(buf), CV_LOAD_IMAGE_COLOR);
    cv::imwrite("save_ocv_result.jpg", img_decode);
 
    return 0;
}
