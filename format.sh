find src tools demos -name \*.c -exec clang-format -i {} \;
find src tools demos -name \*.h -exec clang-format -i {} \;
find src tools demos -name \*.cc -exec clang-format -i {} \;
find src tools demos -name \*.cpp -exec clang-format -i {} \;

for f in tests/*.cpp tests/*.c tests/*.cc
do
  echo "clang-format -i $f"
  clang-format -i $f
done

