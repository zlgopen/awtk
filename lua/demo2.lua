function on_click(ctx, evt) 
  print('on_click');
end

function application_init()
  local win = Window.open("window1");

  win.inc:on(EventType.CLICK, function(evt) 
    win.bar1:set_value(win.bar1.value + 10);
  end);
  
  win.dec:on(EventType.CLICK, function(evt) 
    win.bar1:set_value(win.bar1.value - 10);
  end);
  
  Timer.add(function() 
    if win.bar2.value <= 95 then
      win.bar2:set_value(win.bar2.value + 5);
    end
    return Ret.REPEAT
  end, 500)
  
  win.dialog:on(EventType.CLICK, function(evt) 
    local dlg = Dialog.open("dialog1");

    dlg.client.ok:on(EventType.CLICK, function(evt)
      dlg:quit(0)
    end);

    dlg.client.cancel:on(EventType.CLICK, function(evt)
      dlg:quit(1)
    end);

    dlg:modal()
    dlg:destroy()
  end);
end

application_init()


