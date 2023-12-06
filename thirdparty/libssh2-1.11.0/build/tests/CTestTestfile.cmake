# CMake generated Testfile for 
# Source directory: D:/apps/filefile/thirdparty/libssh2-1.11.0/tests
# Build directory: D:/apps/filefile/thirdparty/libssh2-1.11.0/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(test_simple "D:/apps/filefile/thirdparty/libssh2-1.11.0/build/tests/Debug/test_simple.exe")
  set_tests_properties(test_simple PROPERTIES  ENVIRONMENT "srcdir=D:/apps/filefile/thirdparty/libssh2-1.11.0/tests" _BACKTRACE_TRIPLES "D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;116;add_test;D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(test_simple "D:/apps/filefile/thirdparty/libssh2-1.11.0/build/tests/Release/test_simple.exe")
  set_tests_properties(test_simple PROPERTIES  ENVIRONMENT "srcdir=D:/apps/filefile/thirdparty/libssh2-1.11.0/tests" _BACKTRACE_TRIPLES "D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;116;add_test;D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(test_simple "D:/apps/filefile/thirdparty/libssh2-1.11.0/build/tests/MinSizeRel/test_simple.exe")
  set_tests_properties(test_simple PROPERTIES  ENVIRONMENT "srcdir=D:/apps/filefile/thirdparty/libssh2-1.11.0/tests" _BACKTRACE_TRIPLES "D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;116;add_test;D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(test_simple "D:/apps/filefile/thirdparty/libssh2-1.11.0/build/tests/RelWithDebInfo/test_simple.exe")
  set_tests_properties(test_simple PROPERTIES  ENVIRONMENT "srcdir=D:/apps/filefile/thirdparty/libssh2-1.11.0/tests" _BACKTRACE_TRIPLES "D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;116;add_test;D:/apps/filefile/thirdparty/libssh2-1.11.0/tests/CMakeLists.txt;0;")
else()
  add_test(test_simple NOT_AVAILABLE)
endif()
