find src tools demos tests -name \*.c -exec clang-format -i {} \;
find src tools demos tests -name \*.h -exec clang-format -i {} \;
find src tools demos tests -name \*.cc -exec clang-format -i {} \;
find src tools demos tests -name \*.cpp -exec clang-format -i {} \;

