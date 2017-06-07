#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/data_buffer.o \
	${OBJECTDIR}/src/event_data.o \
	${OBJECTDIR}/src/event_loop.o \
	${OBJECTDIR}/src/net_package.o \
	${OBJECTDIR}/src/safe_queue.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-std=c++11 -g -fPIC -lglog
CXXFLAGS=-std=c++11 -g -fPIC -lglog

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ../lib/libnet.a

../lib/libnet.a: ${OBJECTFILES}
	${MKDIR} -p ../lib
	${RM} ../lib/libnet.a
	${AR} -rv ../lib/libnet.a ${OBJECTFILES} 
	$(RANLIB) ../lib/libnet.a

${OBJECTDIR}/src/data_buffer.o: src/data_buffer.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/data_buffer.o src/data_buffer.cc

${OBJECTDIR}/src/event_data.o: src/event_data.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/event_data.o src/event_data.cc

${OBJECTDIR}/src/event_loop.o: src/event_loop.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/event_loop.o src/event_loop.cc

${OBJECTDIR}/src/net_package.o: src/net_package.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/net_package.o src/net_package.cc

${OBJECTDIR}/src/safe_queue.o: src/safe_queue.cc 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} "$@.d"
	$(COMPILE.cc) -g -Iinclude -I../common/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/src/safe_queue.o src/safe_queue.cc

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ../lib/libnet.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc