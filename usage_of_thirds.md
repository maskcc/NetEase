1. glog, 是谷歌开源的日志工具,使用方法[参考](http://www.cnblogs.com/hiloves/p/6009707.html)

1、使用glog之前必须先初始化库，要生成日志文件只需在开始log之前调用一次：

google::InitGoogleLogging(argv[0]);  //括号内是程序名
当要结束glog时必须关闭库，否则会内存溢出：

google::ShutdownGoogleLogging();

2、设置日志文件保存目录，这个目录必须是已经存在的，否则不能生成日志文件。必须在初始化库之前调用。

FLAGS_log_dir = "c:\\Logs";

14、GLOG 有四个错误级别，枚举如下：

enum SeverityLevel
{
	　　google::INFO = 0,
	　　google::WARNING = 1,
	　　google::ERROR = 2,
	　　google::FATAL = 3,
};

3、输出日志：

LOG(INFO) << "info test";  //输出一个Info日志
LOG(WARNING) << "warning test";  //输出一个Warning日志
LOG(ERROR) << "error test";  //输出一个Error日志
LOG(FATAL) << "fatal test";  //输出一个Fatal日志，这是最严重的日志并且输出之后会中止程序

4、条件输出：

LOG_IF(INFO, num_cookies > 10) << "Got lots of cookies";  //当条件满足时输出日志

LOG_EVERY_N(INFO, 10) << "Got the " << google::COUNTER << "th cookie";  
//google::COUNTER 记录该语句被执行次数，从1开始，在第一次运行输出日志之后，每隔 10 次再输出一次日志信息

LOG_IF_EVERY_N(INFO, (size > 1024), 10) << "Got the " << google::COUNTER << "th big cookie";  
//上述两者的结合，不过要注意，是先每隔 10 次去判断条件是否满足，如果滞则输出日志；
//而不是当满足某条件的情况下，每隔 10 次输出一次日志信息

LOG_FIRST_N(INFO, 20) << "Got the " << google::COUNTER << "th cookie";  
//当此语句执行的前 20 次都输出日志，然后不再输出

5、几个函数：

google::SetLogDestination(google::GLOG_INFO, "log/prefix_");  
//设置特定严重级别的日志的输出目录和前缀。第一个参数为日志级别，第二个参数表示输出目录及日志文件名前缀

google::SetLogFilenameExtension("logExtension");  //在日志文件名中级别后添加一个扩展名。适用于所有严重级别

google::SetStderrLogging(google::GLOG_INFO);  //大于指定级别的日志都输出到标准输出

6、几个参数设置：

FLAGS_logtostderr = true;  //设置日志消息是否转到标准输出而不是日志文件
FLAGS_alsologtostderr = true;  //设置日志消息除了日志文件之外是否去标准输出
FLAGS_colorlogtostderr = true;  //设置记录到标准输出的颜色消息（如果终端支持）
FLAGS_log_prefix = true;  //设置日志前缀是否应该添加到每行输出
FLAGS_logbufsecs = 0;  //设置可以缓冲日志的最大秒数，0指实时输出
FLAGS_max_log_size = 10;  //设置最大日志文件大小（以MB为单位）
FLAGS_stop_logging_if_full_disk = true;  //设置是否在磁盘已满时避免日志记录到磁盘

7、例子：

// Start google log system:
FLAGS_log_dir = "c:\\Logs";
google::InitGoogleLogging(argv[0]);
google::SetLogDestination(google::GLOG_INFO, "c:\\Logs\\INFO_");
google::SetStderrLogging(google::GLOG_INFO);
google::SetLogFilenameExtension("log_");
FLAGS_colorlogtostderr = true;  // Set log color
FLAGS_logbufsecs = 0;  // Set log output speed(s)
FLAGS_max_log_size = 1024;  // Set max log file size
FLAGS_stop_logging_if_full_disk = true;  // If disk is full
char str[20] = "hello log!";
LOG(INFO) << str;
CStringA cStr = "hello google!";
LOG(INFO) << cStr;
LOG(INFO) << "info test" << "hello log!";  //输出一个Info日志
LOG(WARNING) << "warning test";  //输出一个Warning日志
LOG(ERROR) << "error test";  //输出一个Error日志
google::ShutdownGoogleLogging();

2. Valgrind 是内存泄漏检查工具,使用方法[参考](http://valgrind.org/docs/manual/quick-start.html)

#如果程序是这样的 myprog arg1 arg2 使用如下:
valgrind --leak-check=yes myprog arg1 arg2
here are several kinds of leaks; the two most important categories are:

"definitely lost": your program is leaking memory -- fix it!

"probably lost": your program is leaking memory, unless you're doing funny things with pointers (such as moving them to point to the 
		middle of a heap block).

3. msgpack 的用法, 这里用的是用法三, 使用 MSGPACK_DEFINE 来使用类解析

			# Implementation Status

			The serialization library is production-ready.

			Currently, RPC implementation is in testing phase. Requires newer kernel, not running on RHEL5/CentOS5.

			# Install

			Same as QuickStart for C Language.

			# Serialization QuickStart for C+\+

			## First program

			Include `msgpack.hpp` header and link `msgpack` library to use MessagePack on your program.

			```cpp
			#include <msgpack.hpp>
			#include <vector>
			#include <string>
			#include <iostream>

			int main(void) {
				// serializes this object.
				std::vector<std::string> vec;
				vec.push_back("Hello");
				vec.push_back("MessagePack");

				// serialize it into simple buffer.
				msgpack::sbuffer sbuf;
				msgpack::pack(sbuf, vec);

				// deserialize it.
				msgpack::object_handle oh =
					msgpack::unpack(sbuf.data(), sbuf.size());

				// print the deserialized object.
				msgpack::object obj = oh.get();
				std::cout << obj << std::endl;  //=> ["Hello", "MessagePack"]

				// convert it into statically typed object.
				std::vector<std::string> rvec;
				obj.convert(rvec);
			}
			```

			Compile it as follows:

			```
			$ g++ -Ipath_to_msgpack/include hello.cc -o hello
			$ ./hello
			["Hello", "MessagePack"]
			```

			## Streaming feature

			```cpp
			#include <msgpack.hpp>
			#include <iostream>
			#include <string>

			int main(void) {
				// serializes multiple objects using msgpack::packer.
				msgpack::sbuffer buffer;

				msgpack::packer<msgpack::sbuffer> pk(&buffer);
				pk.pack(std::string("Log message ... 1"));
				pk.pack(std::string("Log message ... 2"));
				pk.pack(std::string("Log message ... 3"));

				// deserializes these objects using msgpack::unpacker.
				msgpack::unpacker pac;

				// feeds the buffer.
				pac.reserve_buffer(buffer.size());
				memcpy(pac.buffer(), buffer.data(), buffer.size());
				pac.buffer_consumed(buffer.size());

				// now starts streaming deserialization.
				msgpack::object_handle oh;
				while(pac.next(oh)) {
					std::cout << oh.get() << std::endl;
				}

				// results:
				// $ g++ -Ipath_to_msgpack/include stream.cc -o stream
				// $ ./stream
				// "Log message ... 1"
				// "Log message ... 2"
				// "Log message ... 3"
			}
			```

			### Streaming into an array or map

			```cpp
			#include <msgpack.hpp>
			#include <iostream>
			#include <string>

			int main(void) {
				// serializes multiple objects into one message containing an array using msgpack::packer.
				msgpack::sbuffer buffer;

				msgpack::packer<msgpack::sbuffer> pk(&buffer);
				pk.pack_array(3);
				pk.pack(std::string("Log message ... 1"));
				pk.pack(std::string("Log message ... 2"));
				pk.pack(std::string("Log message ... 3"));

				// serializes multiple objects into one message containing a map using msgpack::packer.
				msgpack::sbuffer buffer2;

				msgpack::packer<msgpack::sbuffer> pk2(&buffer2);
				pk2.pack_map(2);
				pk2.pack(std::string("x"));
				pk2.pack(3);
				pk2.pack(std::string("y"));
				pk2.pack(3.4321);

			}
			```


			## User-defined classes

			You can use serialize/deserializes user-defined classes using `MSGPACK_DEFINE` macro.

			```cpp
			#include <msgpack.hpp>
			#include <vector>
			#include <string>

			class myclass {
				private:
					std::string m_str;
					std::vector<int> m_vec;
				public:
					MSGPACK_DEFINE(m_str, m_vec);
			};

			int main(void) {
				std::vector<myclass> vec;
				// add some elements into vec...

				// you can serialize myclass directly
				msgpack::sbuffer sbuf;
				msgpack::pack(sbuf, vec);

				msgpack::object_handle oh =
					msgpack::unpack(sbuf.data(), sbuf.size());

				msgpack::object obj = oh.get();

				// you can convert object to myclass directly
				std::vector<myclass> rvec;
				obj.convert(rvec);
			}
			```
