function on_click(ctx, evt) 
  print('on_click');
end

function application_init()
  local win = Window.create(nil, 0, 0, 0, 0)
  local inc = Button.create(win, 10, 5, 80, 30)
  inc:set_text(to_wstr('Inc'));
 
  local dec = Button.create(win, 100, 5, 80, 30);
  dec:set_text(to_wstr('Dec'));
  
  local bar = ProgressBar.create(win, 10, 80, 168, 30);
  bar:set_value(40);
  
  inc:on(EventType.CLICK, function(evt) 
    local e = PointerEvent.cast(evt);
    bar:set_value(bar.value + 10);
    print('on inc click:(' .. tostring(e.x) .. ' ' .. tostring(e.y) .. ')')
  end);
  
  dec:on(EventType.CLICK, function(evt) 
    local e = PointerEvent.cast(evt);
    bar:set_value(bar.value - 10);
    print('on dec click:(' .. tostring(e.x) .. ' ' .. tostring(e.y) .. ')')
  end);
  
  local bar1 = ProgressBar.create(win, 10, 118, 168, 30);
  bar1:set_value(40);

  Timer.add(function() 
    if bar1.value > 95 then
      bar1:set_value(0);
    else
      bar1:set_value(bar1.value + 5);
    end
    return Ret.REPEAT
  end, 500)
end

application_init()


