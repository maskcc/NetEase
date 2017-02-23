TARGETNAME := protocols
TARGETPATH := ./bin
OBJECTPATH := ./obj/$(TARGETNAME)
SRCPATH := ./protocols
# 0 -- Application; 1 -- Static library; 2 -- Dynamic library
TARGETTYPE = 1
TARGETINCS = -I./include  -I../common/include
TARGETLIBS =  #../protocol/OdaoMobileProto.a

CPP = g++
DEBUG = -g -DDEBUG -D_REENTRANT
RELEASE = -g -O2
CPPFLAGS  = $(TARGETINCS) -std=c++11
LINKFLAGS = $(TARGETLIBS) -lpthread -lrt -lnsl -lgcc_s  
TARGETSRCS = $(wildcard $(SRCPATH)/*.c $(SRCPATH)/*.cc)
TARGETOBJSRCS = $(subst $(SRCPATH)/, ,$(TARGETSRCS))
OBJS = $(addprefix $(OBJECTPATH)/, $(patsubst %.c,%.o,$(TARGETOBJSRCS:.cc=.o)))

ifeq ($(strip $(TARGETPATH)),)
    TARGETPATH := .
endif

ifeq ($(strip $(OBJECTPATH)),)
    OBJECTPATH := .
endif

ifneq ($(strip $(MAKECMDGOALS)), release)
    CPPFLAGS := $(CPPFLAGS) $(DEBUG)
else
    CPPFLAGS := $(CPPFLAGS) $(RELEASE)
endif

$(OBJECTPATH)/%.o : $(SRCPATH)/%.cc;mkdir -p $(OBJECTPATH);$(CPP) $(CPPFLAGS) -c $< -o $@
$(OBJECTPATH)/%.o : $(SRCPATH)/%.c;mkdir -p $(OBJECTPATH);$(CPP) $(CPPFLAGS) -c $< -o $@

ifneq ($(TARGETTYPE), 1)
    ifeq ($(TARGETTYPE), 2)
        TYPE = -shared
        TARGETNAME := $(TARGETNAME).so
    endif
    $(TARGETNAME) : $(OBJS)
	@mkdir -p $(TARGETPATH)
	$(CPP) $(CPPFLAGS) -o $(TARGETPATH)/$@ $^ $(LINKFLAGS) $(TYPE)
	chmod 755 $(TARGETPATH)/$(TARGETNAME)
else
    TARGETNAME := $(TARGETNAME).a
    $(TARGETNAME) : $(OBJS)
	@mkdir -p $(TARGETPATH)
	$(AR) rcs $(TARGETPATH)/$@ $^
endif

.PHONY: all debug release clean run gdb w

all debug release: $(TARGETNAME)

clean:
	rm -f $(OBJECTPATH)/*.o $(TARGETPATH)/$(TARGETNAME) $(TARGETPATH)/core*
run:	
	cd $(TARGETPATH);./run.sh
	
gdb:	
	cd $(TARGETPATH);gdb $(TARGETNAME)
	
w:
	vim src/main.cc
