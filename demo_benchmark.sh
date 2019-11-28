#!/bin/bash

ri=8 # restart_interval

echo "cpp encode"
./build/encode images/a.png ${ri}
echo

echo "python encode"
python3 encode_demo.py images/a.png ${ri}
echo

echo "cpp decode"
./build/decode images/a.png ${ri}
echo

echo "python decode"
python3 decode_demo.py images/a.png ${ri}
echo

