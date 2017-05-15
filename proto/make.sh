#用makefile 来编写, 要编译与清除dest目录的内容
SRC_DIR=./src
DST_DIR=./protocols
CSP_DIR=./csharp_proto
mkdir -p ${DST_DIR}
mkdir -p ${CSP_DIR}

protoc  -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/book.proto
protoc  -I=$SRC_DIR --csharp_out=$CSP_DIR $SRC_DIR/book.proto



