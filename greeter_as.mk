# greeter-as Makefile

all: stage-build

BUILD_DIR := $(abspath $(shell pwd))/build
ROOT := $(abspath $(shell pwd)/../..)
MK_DIR := ${ROOT}/mk

TARGET := greeter-as.so
TARGET_TEST := greeter-as.so_test

TARGET_SOURCES := greeterappserver.cpp \
		  greeterplugin.cpp

CPPFLAGS += -Wno-write-strings \
	    -Werror \
            -ggdb3
CXXFLAGS += -std=c++11

# Build location independent code for shared object
CPPFLAGS += -fpic
CPPFLAGS += -I${ROOT}/include \
            -I${ROOT}/usr/include \
            -I${ROOT}/plugins/greeter/include \
            -I${ROOT}/modules/cpp-common/include \
            -I${ROOT}/modules/app-servers/include \
	    -I${ROOT}/modules/rapidjson/include

CPPFLAGS += $(shell PKG_CONFIG_PATH=${ROOT}/usr/lib/pkgconfig pkg-config --cflags libpjproject)

# Add greeter/src as VPATH so build will find modules there.
VPATH = ${ROOT}/sprout:${ROOT}/plugins/greeter/src

# Production build:
#
# Enable optimization in production only.
CPPFLAGS := $(filter-out -O2,$(CPPFLAGS))
CPPFLAGS_BUILD += -O2

LDFLAGS += -L${ROOT}/usr/lib -shared

include ${MK_DIR}/platform.mk

.PHONY: stage-build
stage-build: build

.PHONY: distclean
distclean: clean
