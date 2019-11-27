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

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

namespace py = pybind11;


class FastEncoder{
public:

    int encode(cv::Mat ocv_img, std::vector<uint8_t> &buf){
        return encode(ocv_img.data, buf);
    }


    int encode(uint8_t* ocv_img_data, std::vector<uint8_t> &buf){
        buf = encode_direct(ocv_img_data);
        return 0;
    }


    std::vector<uint8_t> encode_npy(py::array_t<uint8_t> img_np){
        auto data_buf = img_np.request();
        return encode_direct( (uint8_t *) data_buf.ptr);
    }

    std::vector<uint8_t> encode_direct(uint8_t* ocv_img_data){
        gpujpeg_encoder_input_set_image(&encoder_input, ocv_img_data);

        int image_compressed_size = 0;

        if ( gpujpeg_encoder_encode(encoder, &param, &param_image, &encoder_input, &image_compressed, &image_compressed_size) != 0 ) {
            fprintf(stderr, "Failed to encode image!\n");
            // return -1;
        }

        return std::vector<uint8_t>(image_compressed, image_compressed+image_compressed_size);

        // char* output = "save_direct.jpg"
        // if ( gpujpeg_image_save_to_file(output, image_compressed, image_compressed_size) != 0 ) {
        //     fprintf(stderr, "Failed to save image!\n");
        //     return -1;
        // }
    }

    FastEncoder(int width = 1280, int height = 720, int device_id = 0){

        fprintf(stdout, "GPU encoder init");

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

