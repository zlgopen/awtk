function on_click(ctx, evt) 
  print('on_click');
end

function application_init()
  local win = Window.create(NULL, 0, 0, 0, 0);
  local ok = Button.create(win, 10, 5, 80, 30);
  ok:set_text(to_wstr("ok"));
  
  rid = ok:on(EventType.EVT_CLICK, function(evt) 
    print("on click:" .. tostring(rid))
    ok:off(rid)
  end);

  print(tostring(rid))
end

application_init()


