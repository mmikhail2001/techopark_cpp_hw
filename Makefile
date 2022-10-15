# чувствителен к изменению исходных файлов
# игнорирует изменения файла CMakeLists.txt (если make run)

LIB = matrix_lib
BUILD_DIR = build
TARGET = ./${BUILD_DIR}/main
TEST_OPT 		= OFF
DEBUG_OPT 		= OFF

.PHONY: build rebuild test run

build: clean
	mkdir ${BUILD_DIR}
	cd ${BUILD_DIR} && cmake .. -DTEST_OPT=${TEST_OPT} -DDEBUG_OPT=${DEBUG_OPT} && $(MAKE) --no-print-directory

clean: 
	(rm -r ${BUILD_DIR} 2>/dev/null) || exit 0 

# выполняется, если проект собран
run:
	cd ${BUILD_DIR} && $(MAKE) --no-print-directory
	${TARGET}

# выполняется, если проект собран
test: ${TARGET}
	./${BUILD_DIR}/${LIB}/tests/tests
# cd ${BUILD_DIR}/${LIB} && ctest

