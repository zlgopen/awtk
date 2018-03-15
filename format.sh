find src tools  lua demos tests -name \*.c -exec clang-format -i {} \;
find src tools lua demos tests -name \*.cc -exec clang-format -i {} \;
find src tools lua demos tests -name \*.cpp -exec clang-format -i {} \;

