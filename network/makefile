CXX := g++
#0表示编译成可执行文件 1 表示打包成静态库, 2表示打包成动态库
PACK := 1
CC := cc
CXXFLAGS := -g -D_REENTRANT -DDEBUG -std=c++11 -fPIC
src_path := src
lib_path := -L../lib
inc_path := -I./include -I../net/include -I../common/include
libs = -lpthread -lrt -lnsl -lgcc_s -lnet -lglog
target_path := ../lib
target := libnet.a
obj_path:= ../objs/$(target)


srcs := $(wildcard $(src_path)/*.$(CC))
#objs := $(srcs:$(src_path)/%.$(CC)=$(obj_path)/%.o)
objs := $(addprefix $(obj_path)/, $(addsuffix .o, $(basename $(notdir $(srcs))) ))
dest := $(target_path)/$(target)
ifeq ($(PACK), 0)
$(dest): $(objs)
	$(CXX)  $(CXXFLAGS) -o $(dest) $(objs) $(lib_path) $(libs)
endif

ifeq ($(PACK), 1)
$(dest): $(objs)
	$(AR) r $(dest) $(objs) 
endif

ifeq ($(PACK), 2)
$(dest): $(objs)
	$(CXX)  $(CXXFLAGS) -o $(dest) $(objs) $(lib_path) $(libs) -shared

endif

$(obj_path)/%.o:$(src_path)/%.$(CC)
	$(CXX) $(inc_path) $(CXXFLAGS) -c $< -o $@ 

.PHONE : clean test run pre 
test:
	@echo $(objs)
	@echo $(srcs)

run: 
	./bin/main
clean :
	-rm -f $(objs) $(dest)

pre:
	@mkdir -p $(target_path)
	@mkdir -p $(obj_path)

