<center><h1>libuv</h1></center>
<p>
The idea is to create a c++11 binding of <a href="https://github.com/libuv/libuv">libuv</a> using c++11 functional features and boost.
</p>

<p>
This project is in a very early stage of development. after we - oh who am i kidding - after I finish implementing all the following features and exhaustive testing i may release a beta version.
so for the mean time use at your own risk.
</p>
<h5>
supported features : (Synchronously and Asynchronously)
</h5>

* FS
 * read
 * write
 * unlink
 * file watchers
 * ~~mkdir/rmdir~~
 * ~~ls~~
 * ~~stat~~
* ~~Networking~~
 * ~~TCP~~
 * ~~UDP~~
 * ~~DNS lookup~~
* ~~Processes~~
 * ~~Fork~~
 * ~~Signals~~
 * ~~IPC~~
 * ~~Pipes~~

<h5>
Build Instructions :
</h5>
`git clone --recursive git@github.com:heshamsafi/cpuv.git #recursive to clone the submodule(libuv) aswell`

`cd deps/libuv`

`./gyp_uv.py -f make # for some reason can't build libuv with autotools`

`make -C out/`

`cd ../..`

`./configure`

`automake --add-missing`

`make`

`make check # to run unit tests`

this build is done on linux but in theory this project should be portable.

<h5>
Shout Out:
</h5>
<p>
for all of you C++ devs out there, I need help with this one.
</p>
