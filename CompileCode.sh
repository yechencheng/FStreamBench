#!/bin/sh

rm -f FFT.out
rm -f FFTOpt.out

echo "=====COMPILE FFT VERSION====="
strc StubFFT.str FFT.str Statics.str -cluster 4 -o FFT.out
echo "=====COMPILE FFT OPT VERSION====="
strc StubFFT.str FFT_HandOpt.str Statics.str -cluster 4 -o FFTOpt.out
