<center><h1>libuv</h1></center>
<p>
The idea is to create a c++11 binding of <a href="https://github.com/libuv/libuv">libuv</a> using c++11 functional features and boost.
</p>

This project is a very early stage of development. after we - oh who am i kidding - after I finish implementing all the following features and exhaustive testing i may release a beta version.
so for the mean time use at your own risk.
<h5>
supported features : (Synchronously and Asynchronously)
</h5>
<ul>
  <li>FS
     <ul>
       <li>read</li>
       <li>write</li>
       <li>unlink</li>
       <li>~~file watchers~~</li>
       <li>~~mkdir/rmdir~~</li>
       <li>~~ls~~</li>
       <li>~~stat~~</li>
     </ul>
  </li>
  <li>
    ~~Networking~~
    <ul>
      <li>~~TCP~~</li>
      <li>~~UDP~~</li>
      <li>~~DNS lookup~~</li>
    </ul>
  </li>
  <li>
   ~~Processes~~
    <ul>
      <li>~~Fork~~</li>
      <li>~~Signals~~</li>
      <li>~~IPC~~</li>
      <li>~~Pipes~~</li>
    </ul>
  </li>
</ul>

<h5>
Build Instructions :
</h5>
<code>
git clone --recursive git@github.com:heshamsafi/cpuv.git #recursive to clone the submodule(libuv) aswell<br/>
cd deps/libuv <br/>
./gyp_uv.py -f make # for some reason can't build libuv with autotools<br />
make -C out/ <br />
cd ../.. <br />
./configure <br />
automake --add-missing <br />
make <br />
make check # to run unit tests <br />
</code>
this build is done on linux but in theory this project should be portable.

<h5>
Shout Out:
</h5>
for all of you C++ devs out there, I need help with this one.
