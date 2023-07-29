./bin/romfs_make tests foo
find tests | sed 's/^[^/]*\///' | while read -r file
do
  ./bin/romfs_read foo $file t

  if [ -d "tests/$file" ]; then
    echo "$file 是目录"
  else
    result=$(cmp tests/$file t)
    if [ $? -eq 0 ]; then
        echo "文件相同"
    else
        echo "tests/$file t 不同"
    fi
  fi
done

rm -f foo.* t

./bin/romfs_make tests foo true

find tests | while read -r file
do
  ./bin/romfs_read foo $file t

  if [ -d "$file" ]; then
    echo "$file 是目录"
  else
    result=$(cmp $file t)
    if [ $? -eq 0 ]; then
        echo "文件相同"
    else
        echo "$file t 不同"
    fi
  fi
done

rm -f foo.* t
