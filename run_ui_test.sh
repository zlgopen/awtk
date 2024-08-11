#!/bin/bash

function runTest() {
  for f in \
  screen_saver.ini \
  fps.ini \
  main_snapshot.ini \
  main_source.ini \
  max_fps.ini \
  overlay.ini \
  exec_fscript.ini \
  create_widget.ini \
  create_window.ini \
  send_key_events.ini \
  send_pointer_events.ini \
  get_loaded_assets_info.ini \
  get_loaded_image_info.ini \
  language.ini \
  theme.ini \
  basic.ini \
  button.ini \
  edit.ini \
  keyboard.ini \
  toast.ini 
  do
    echo "==============tests/ui_test_data/demouiold/$f=================="
    ./bin/ui_test tests/ui_test_data/demouiold/$f
  done
}


# 检查传入参数的数量
if [ $# -lt 1 ]; then
    echo "Usage: $0 <n times>"
    exit 1
fi

n=$1

for ((i = 0; i < n; i++)); do
    echo "### Execution $((i + 1))/$n ..."
    runTest
done

