#用makefile 来编写, 要编译与清除dest目录的内容
SRC_DIR=./src
DST_DIR=./protocols
protoc  -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/book.proto



