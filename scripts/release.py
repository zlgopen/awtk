import os
import sys
import release_common as common


if len(sys.argv) < 2:
    print("Usage: python " + sys.argv[0] + ' [exe name]')
    print("  Ex: python " + sys.argv[0] + ' demoui.exe')
    sys.exit(0)

common.init(sys.argv[1])
common.release()
