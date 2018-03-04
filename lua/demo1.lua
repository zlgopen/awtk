function on_click(ctx, evt) 
  print('on_click');
end

function application_init()
  local win = Window.create(nil, 0, 0, 0, 0)
  local inc = Button.create(win, 10, 5, 80, 30)
  inc:set_text(to_wstr('Inc'));
 
  local dec = Button.create(win, 100, 5, 80, 30);
  dec:set_text(to_wstr('Dec'));
  
  local progress_bar = ProgressBar.create(win, 10, 80, 168, 30);
  progress_bar:set_value(40);
  
  inc:on(EventType.EVT_CLICK, function(evt) 
    local e = PointerEvent.cast(evt);
    progress_bar:set_value(progress_bar.value + 10);
    print('on inc click:(' .. tostring(e.x) .. ' ' .. tostring(e.y) .. ')')
  end);
  
  dec:on(EventType.EVT_CLICK, function(evt) 
    local e = PointerEvent.cast(evt);
    progress_bar:set_value(progress_bar.value - 10);
    print('on dec click:(' .. tostring(e.x) .. ' ' .. tostring(e.y) .. ')')
  end);
end

application_init()


