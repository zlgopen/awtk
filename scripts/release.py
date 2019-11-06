import os
import sys
import release_common as common


if len(sys.argv) < 2:
    print("Usage: python " + sys.argv[0] + ' exe [assets root] [bin root]')
    print("  Ex: python " + sys.argv[0] + ' demoui.exe')
    sys.exit(0)

CWD = os.getcwd()
bin_root = CWD
assets_root = CWD

exe_name = sys.argv[1]

if len(sys.argv) > 2:
    assets_root = sys.argv[2]

if len(sys.argv) > 3:
    bin_root = sys.argv[3]

common.init(exe_name, assets_root, bin_root)
common.release()
