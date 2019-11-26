/**
 * Fast GPU JPEG encoder example class with [CESNET/GPUJPEG](https://github.com/CESNET/GPUJPEG)
 * compatible/call with OpenCV
 * DongxuMiao@DeepMotion.ai
 *
 * TODO
 * Use OpenCV cv::cuda::GpuMat directly
 */

#include <libgpujpeg/gpujpeg.h>
#include <libgpujpeg/gpujpeg_util.h>

#include <opencv2/opencv.hpp>
#include <opencv2/cudaimgproc.hpp> 
#include <string>
#include <chrono>


class FastEncoder{
public:
    int encode(cv::Mat ocv_img, std::vector<uint8_t> &buf){
        gpujpeg_encoder_input_set_image(&encoder_input, ocv_img.data);

        int image_compressed_size = 0;


        if ( gpujpeg_encoder_encode(encoder, &param, &param_image, &encoder_input, &image_compressed, &image_compressed_size) != 0 ) {
            fprintf(stderr, "Failed to encode image!\n");
            return -1;
        }

        buf = std::vector<uint8_t>(image_compressed, image_compressed+image_compressed_size);

        // char* output = "save_direct.jpg"
        // if ( gpujpeg_image_save_to_file(output, image_compressed, image_compressed_size) != 0 ) {
        //     fprintf(stderr, "Failed to save image!\n");
        //     return -1;
        // }
        return 0;
    }

    FastEncoder(int width = 1280, int height = 720, int device_id = 0){

        gpujpeg_set_default_parameters(&param);
        gpujpeg_image_set_default_parameters(&param_image);

        struct gpujpeg_image_parameters param_image_original;
        gpujpeg_image_set_default_parameters(&param_image_original);

        int component_range = 0;

        param_image.width = width;
        param_image.height = height;

        param_image.color_space = GPUJPEG_RGB;
        param_image.comp_count = 3;
        param_image.pixel_format = GPUJPEG_444_U8_P012;

        param.quality = 95;
        param.restart_interval = 32;

        int flags = GPUJPEG_VERBOSE;
        if ( gpujpeg_init_device(device_id, flags) != 0 ) {
            fprintf(stderr, "Init on gpu device error\n");
            
        }

        if (param_image.width <= 0 || param_image.height <= 0) {
            fprintf(stderr, "Image dimensions must be set to nonzero values!\n");
        }

        encoder = gpujpeg_encoder_create(NULL);
        if ( encoder == NULL ) {
            fprintf(stderr, "Failed to create encoder!\n");
        }
    }

    ~FastEncoder(){
        gpujpeg_image_destroy(image_compressed);
        gpujpeg_encoder_destroy(encoder);
    }

private:

    struct gpujpeg_parameters param;
    struct gpujpeg_image_parameters param_image;
    struct gpujpeg_encoder* encoder = NULL;
    uint8_t* image_compressed = NULL;
    struct gpujpeg_encoder_input encoder_input;
};

int
main(int argc, char *argv[])
{

    if ( argc < 2 ) {
        fprintf(stderr, "Please supply source image filename!\n");
        return -1;
    }

    cv::Mat ocv_img = cv::imread( argv[1], cv::IMREAD_UNCHANGED );
    cv::cvtColor(ocv_img, ocv_img, cv::COLOR_RGB2BGR);
    cv::cuda::GpuMat ocv_img_gpu;
    ocv_img_gpu.upload(ocv_img);

    FastEncoder fastEncoder;

    int iterate = 100;


    auto start = std::chrono::steady_clock::now();
    std::vector<uint8_t> buf;
    for ( int iteration = 0; iteration < iterate; iteration++ ) {
        fastEncoder.encode(ocv_img, buf);
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
