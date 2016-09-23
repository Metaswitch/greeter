TARGETS := greeter-as.so

TEST_TARGETS := greeter_test

ROOT := $(abspath $(shell pwd)/../..)
BUILD_DIR := ${ROOT}/plugins/greeter/build

GREETER_COMMON_SOURCES := greeterappserver.cpp \
                          greeterplugin.cpp \
                          sproutletappserver.cpp

greeter-as.so_SOURCES := ${GREETER_COMMON_SOURCES}

greeter_test_SOURCES := ${GREETER_COMMON_SOURCES} \
                        acr.cpp \
                        base_communication_monitor.cpp \
                        baseresolver.cpp \
                        chronosconnection.cpp \
                        connection_tracker.cpp \
                        custom_headers.cpp \
                        dnscachedresolver.cpp \
                        dnsparser.cpp \
                        fakecurl.cpp \
                        fakehssconnection.cpp \
                        fakelogger.cpp \
                        fakesnmp.cpp \
                        faketransport_udp.cpp \
                        faketransport_tcp.cpp \
                        greeterappserver_test.cpp \
                        hssconnection.cpp \
                        http_connection_pool.cpp \
                        httpclient.cpp \
                        httpconnection.cpp \
                        a_record_resolver.cpp \
                        ifchandler.cpp \
                        load_monitor.cpp \
                        log.cpp \
                        logger.cpp \
                        mock_sas.cpp \
                        namespace_hop.cpp \
                        notify_utils.cpp \
                        pjutils.cpp \
                        quiescing_manager.cpp \
                        saslogger.cpp \
                        sessioncase.cpp \
                        sipresolver.cpp \
                        siptest.cpp \
                        snmp_ip_row.cpp \
                        snmp_row.cpp \
                        stack.cpp \
                        subscriber_data_manager.cpp \
                        test_main.cpp \
                        uri_classifier.cpp \
                        utils.cpp \
                        zmq_lvc.cpp

COMMON_CPP_FLAGS := -I${ROOT}/include \
                    -I${ROOT}/usr/include \
                    -I${ROOT}/modules/app-servers/include \
                    -I${ROOT}/modules/cpp-common/include \
                    -I${ROOT}/modules/rapidjson/include \
                    -I${ROOT}/plugins/greeter/include

greeter-as.so_CPPFLAGS := ${COMMON_CPP_FLAGS} \
                          -fPIC

greeter-as.so_LDFLAGS := -shared -L${ROOT}/usr/lib

greeter_test_CPPFLAGS := ${COMMON_CPP_FLAGS} \
                         -I${ROOT}/modules/rapidjson/include \
                         -I${ROOT}/modules/cpp-common/test_utils \
                         -I${ROOT}/modules/app-servers/test \
                         -I${ROOT}/src/ut \
                         $(shell PKG_CONFIG_PATH=${ROOT}/usr/lib/pkgconfig pkg-config --cflags libpjproject) \
                         -Wno-write-strings

greeter_test_LDFLAGS := -L${ROOT}/usr/lib \
                        -lmemcached \
                        -lmemcachedutil \
                        -lssl \
                        -lcrypto \
                        -ldl \
                        -lwebsocketpp \
                        -lboost_regex \
                        -lboost_system \
                        -lboost_thread \
                        -lboost_date_time \
                        -lcares \
                        -lzmq \
                        -levhtp \
                        -levent \
                        -levent_pthreads \
                        -lcurl \
                        -lpthread \
                        -lsas \
                        -lz \
                        $(shell PKG_CONFIG_PATH=${ROOT}/usr/lib/pkgconfig pkg-config --libs libpjproject) \
                        $(shell net-snmp-config --netsnmp-agent-libs)


VPATH = ${ROOT}/src:${ROOT}/modules/cpp-common/src:${ROOT}/plugins/greeter/src:${ROOT}/plugins/greeter/ut:${ROOT}/modules/cpp-common/test_utils:${ROOT}/src/ut

include ${ROOT}/build-infra/cpp.mk

${BUILD_DIR}/bin/greeter_test : ${greeter_test_OBJECT_DIR}/md5.o \
                                ${greeter_test_OBJECT_DIR}/test_interposer.so

SIPP_DIR := $(ROOT)/modules/sipp
$(greeter_test_OBJECT_DIR)/md5.o : $(SIPP_DIR)/md5.c
	$(CC) $(greeter_test_CPPFLAGS) -I$(SIPP_DIR) -c $(SIPP_DIR)/md5.c -o $@

${greeter_test_OBJECT_DIR}/test_interposer.so : ../modules/cpp-common/test_utils/test_interposer.cpp ../modules/cpp-common/test_utils/test_interposer.hpp
	$(CXX) $(greeter_test_CPPFLAGS) -shared -fPIC -ldl $< -o $@

DEB_COMPONENT := greeter
DEB_MAJOR_VERSION := 1.0${DEB_VERSION_QUALIFIER}
DEB_NAMES += greeter-as greeter-as-dbg

include ${ROOT}/build-infra/cw-deb.mk

.PHONY: deb
deb: all deb-only
