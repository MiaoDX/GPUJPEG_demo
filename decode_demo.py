#!/usr/bin/env python3
# -*- encoding: utf-8 -*-

# Note, restart_interval should exist for GPU to actually work

import sys
import cv2
import time
import numpy as np

sys.path.append('./build')
import GPUJPEG_Encoder
import GPUJPEG_Decoder

if len(sys.argv) < 3:
    print('Usage: python3 encode_demo.py <input_image_file> <restart_interval>')
    exit()

in_f = sys.argv[1]

im = cv2.imread(in_f)

height, width = im.shape[:2]
restart_interval = int(sys.argv[2])
restart_interval_encode = restart_interval
restart_interval_decode = restart_interval

gpu_encoder = GPUJPEG_Encoder.Encoder(height=height,
                                      width=width,
                                      restart_interval=restart_interval_encode)
encode_bytes_buf = gpu_encoder.encode(im)

gpu_decoder = GPUJPEG_Decoder.Decoder(height=height,
                                      width=width,
                                      restart_interval=restart_interval_decode)

N = 100

t0 = time.perf_counter()
for _ in range(N):
    decode_buf = gpu_decoder.decode(encode_bytes_buf, len(encode_bytes_buf))
    im = np.fromstring(decode_buf, np.uint8).reshape(height, width, 3)
print(f'GPU time used:{(time.perf_counter()-t0)*1000: .2f} ms')
cv2.imwrite('decode_gpu_py.jpg', im)

buf_npy = np.fromstring(encode_bytes_buf, np.uint8)
t0 = time.perf_counter()
for _ in range(N):
    im_cv2 = cv2.imdecode(buf_npy, cv2.IMREAD_UNCHANGED)
    im_cv2 = cv2.cvtColor(im_cv2, cv2.COLOR_RGB2BGR)
print(f'CV2 time used:{(time.perf_counter()-t0)*1000: .2f} ms')
cv2.imwrite('decode_cv2_py.jpg', im_cv2)
