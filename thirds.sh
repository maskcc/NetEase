apt-get install -y autoconf automake libtool curl make cmake g++ unzip
mkdir ../third
cd ../third
#1. glog
cd ../third;git clone https://github.com/google/glog.git
#2. libevents 
cd ../third;git clone https://github.com/nmathewson/Libevent.git
#3. protobuf
cd ../third;git clone https://github.com/google/protobuf.git
#4. rapidjson
cd ../third;git clone https://github.com/miloyip/rapidjson.git
#5. svn
apt-get install -y svn
#5.  valgrind
cd ../third; svn co svn://svn.valgrind.org/valgrind/trunk valgrind

cd ../third/glog; autoreconf -ivf;./configure; make; make install
cd ../Libevent;pwd; ./configure; make; make install;ldconfig
cd ../protobuf;pwd; ./autogen.sh;./configure; make; make install;ldconfig
cd ../rapidjson;pwd; git submodule update --init;mkdir build; cd build; cmake .. ;ldconfig
cd ../valgrind;pwd; ./autogen.sh;./configure;make; make install;ldconfig
