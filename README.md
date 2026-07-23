由于MATLAB自带的FFT过于垃圾，给fftw团队写信又毫无回音，一怒之下用Google，百度和deepseek重写了一份。

testFFT运行环境Microsoft Visual C++ 6.0

MATLAB 2018b以上，sine和方波都分别有MATLAB实现与自己实现的代码，读者可自己编译运行，体会他们之间的差别。

MATLAB 2018b以上执行 mex -setup 以安装Microsoft Visual C++编译器

MATLAB 2018b以上执行 mex -setup -V C++ 察看哪些编译器受支持。

MATLAB中执行 mex XXX.c 来编译它为mex文件。

SHA256 的 vs_BuildTools_2015.exe 哈希:
1e1774869abd953d05d10372b7c08bfa0c76116f5c6df1f3d031418ccdcd8f7b

SHA256 的 vs_BuildTools_2017.exe 哈希:
7b48ee28c2c6d0c860d8d7226e25cab85092b593002bc13d4bd42a57310ba9fa

SHA256 的 vs_BuildTools_2019.exe 哈希:
d7e92c7336caf423be3e042851c45d887ff93116cb3df7ffd94bc864bfb01ba8

SHA256 的 vs_BuildTools_2022.exe 哈希:
95b8bcac49757dba2bd80827484e65b2f0ef57c11c143b87514b913839f78657
