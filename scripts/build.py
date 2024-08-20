from utils import *
import sys, os

CONFIG["SRC_PATH"] = "src"
CONFIG["ICON_PATH"] = "src/ico.o"
CONFIG["INCLUDES"] = ["src/", "src/shared/", "src/vendor/"]
CONFIG["DEFINES"] = ["VERSION=2024.1"]
CONFIG["STD"] = "c++20"
CONFIG["OUTPATH"] = "bin\\AO.exe"
CONFIG["PRECOMPILES"] = [
    ("src/aopch.h.gch", "g++ src/aopch.h"), # compile AO precompiled headers
    ("src/ico.o", "windres src/ico.rc -O coff -o src/ico.o") # create ico.o containing the data for AO icon
]

if not sys.argv[1:]:
    CONFIG["OPTIMIZATION"] = "-O2"
    precompile_files()
    compile_ao()

for i, x in enumerate(sys.argv[1:]):
    if x == "clean":
        rm(sys.argv[i+2:] if sys.argv[i+2:] else ["bin/", "obj/", "src/aopch.h.gch", "src/ico.o"])

    elif x == "pch":
        precompile_files()

    elif x == "run":
        CONFIG["OPTIMIZATION"] = ""
        compile_ao()
        input("press enter to continue.")
        run_ao(sys.argv[i+2:])

    elif x == "exec":
        run_ao(sys.argv[i+2:])
        break
