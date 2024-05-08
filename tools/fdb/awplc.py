import os
import lldb

class StopHook:
    def __init__(self, target, extra_args1, extra_args2):
        self.target = target

    def handle_stop(self, exe_ctx, stream):
        name = exe_ctx.GetFrame().GetFunctionName()
        if name != None :
            print('stop in: ' + name);
            if name.find('AWPLC_APP::') != -1:
                return True
        return False
print('load awplc.py');