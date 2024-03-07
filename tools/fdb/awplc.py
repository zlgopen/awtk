import os
import lldb

if not 'AWPLC_USER_FUNCTIONS' in os.environ:
    os.environ["AWPLC_USER_FUNCTIONS"] = "awplc_user_functions.txt"

if os.path.isfile(os.environ["AWPLC_USER_FUNCTIONS"]) == False:
    print("File awplc_user_functions.txt not found")

class StopHook:
    def __init__(self, target, extra_args1, extra_args2):
        self.target = target

    def handle_stop(self, exe_ctx, stream):
        name = exe_ctx.GetFrame().GetFunctionName()
        print('stop in: ' + name);
        if name.find('AWPLC_APP::') != -1:
            return True
        return False
print('load awplc.py');
