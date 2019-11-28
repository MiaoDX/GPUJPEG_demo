/*
* Export encode into python binding
*/

#include "decode.h"

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// ----------------
// Python interface
// ----------------

PYBIND11_MODULE(GPUJPEG_Decoder, m) {
  m.doc() = "pybind11 GPUJPEG_Decoder";

  py::class_<FastDecoder>(m, "Decoder")
      .def(py::init<int, int, int, int>(), pybind11::arg("width") = 1280, pybind11::arg("height") = 720,
           pybind11::arg("restart_interval") = 32, pybind11::arg("device_id") = 0)
      .def("decode", &FastDecoder::decode_npy, pybind11::arg("buf"), pybind11::arg("buf_len"));
  ;
}
