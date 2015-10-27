.PHONY: build
build:
	${MAKE} -f greeter_as.mk

.PHONY: test run_test coverage coverage-check coverage_raw debug vg vg-check vg_raw
test run_test coverage coverage-check coverage_raw debug vg vg-check vg_raw:
	${MAKE} -f greeter_test.mk $@

.PHONY: clean
clean: buildclean testclean

.PHONY: buildclean
buildclean:
	${MAKE} -f greeter_as.mk clean

.PHONY: testclean
testclean:
	${MAKE} -f greeter_test.mk clean

DEB_COMPONENT := greeter
DEB_MAJOR_VERSION := 1.0${DEB_VERSION_QUALIFIER}
DEB_NAMES += greeter-as greeter-as-dbg

include ../../build-infra/cw-deb.mk

.PHONY: deb
deb: build deb-only
