function on_click(ctx, evt) 
  print('on_click');
end

function application_init()
  local win = Window.create(NULL, 0, 0, 0, 0);
  ok = Button.create(win, 10, 5, 80, 30);
  ok:set_text(to_wstr("ok"));

  print(ok, getmetatable(ok));
  label = Label.create(win, 100, 5, 80, 30);
  label:set_text(to_wstr("text"));
  print(ok, getmetatable(ok));
  print(label, getmetatable(label));
  p = ProgressBar.create(win, 200, 5, 80, 30);
  print(ok, getmetatable(ok));
  print(label, getmetatable(label));
  print(p, getmetatable(p));

end

application_init()


