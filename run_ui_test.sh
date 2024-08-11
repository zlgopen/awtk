
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

