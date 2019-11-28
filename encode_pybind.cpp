/*
* Export encode into python binding
*/

#include "encode.h"

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>


namespace py = pybind11;

// ----------------
// Python interface
// ----------------


PYBIND11_MODULE(GPUJPEG_Encoder, m)
{
  m.doc() = "pybind11 example plugin";

  py::class_<FastEncoder>(m, "Encoder")
  .def(py::init<int, int, int, int>(), pybind11::arg("width") = 1280, pybind11::arg("height") = 720, 
                                       pybind11::arg("restart_interval") = 32, pybind11::arg("device_id") = 0)
  .def("encode_only", &FastEncoder::encode_only, pybind11::arg("ocv_img_data") = "cv2_iamge np array")
  .def("encode", &FastEncoder::encode_npy, pybind11::arg("ocv_img_data") = "cv2_iamge np array");

//   .def("encode_test", &FastEncoder::encode_test, pybind11::arg("ocv_img_data") = "cv2_iamge np array")
}
