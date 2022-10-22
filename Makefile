LIB_DIR = matrix_lib
BUILD_DIR = build

TARGET = ./${BUILD_DIR}/main
TESTS_EXE = tests
TESTS_DIR = tests

TEST_OPT 		= OFF
DEBUG_OPT 		= OFF

.PHONY: build rebuild test run

build: clean
	mkdir ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake .. -DTEST_OPT=${TEST_OPT} -DDEBUG_OPT=${DEBUG_OPT} && $(MAKE) --no-print-directory

clean: 
	(rm -r ${BUILD_DIR} 2>/dev/null) || exit 0 

run:
	cd ${BUILD_DIR} && $(MAKE) --no-print-directory
	${TARGET}

test:
	./${BUILD_DIR}/${LIB_DIR}/tests/tests

coverage:
	cd ${BUILD_DIR} && lcov -t "testing_${LIB_DIR}" -o coverage.info -c -d ${LIB_DIR}/CMakeFiles \
	&& lcov --remove coverage.info -o coverage.info '/usr/include/*' '/usr/lib/*' \
	&& genhtml -o report coverage.info

valgrind_tests:
	valgrind --tool=memcheck --leak-check=yes --error-exitcode=1 ./${BUILD_DIR}/${LIB_DIR}/${TESTS_DIR}/${TESTS_EXE}

