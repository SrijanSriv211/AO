from utils import *
import sys, os

config.src_path = "src"
config.icon_path = "src/ico.o"
config.includes = ["src/", "src/shared/"]
config.defines = ["VERSION=2024.1"]
config.std = "c++20"
config.outpath = "bin\\AO.exe"
config.filedata = [
    ("src/aopch.h.gch", "g++ src/aopch.h"), # compile AO precompiled headers
    ("src/ico.o", "windres src/ico.rc -O coff -o src/ico.o") # create ico.o containing the data for AO icon
]

if not os.path.isdir("bin"):
    os.mkdir("bin")

for i, x in config.filedata:
    if not os.path.isfile(i):
        precompile_files()
        break

if not sys.argv[1:]:
    config.optimization = "-O2"
    precompile_files()
    compile_ao()

for i, x in enumerate(sys.argv[1:]):
    if x == "clean":
        rm("bin/", "src/aopch.h.gch", "src/ico.o")

    elif x == "pch":
        precompile_files()

    elif x == "run":
        config.optimization = ""
        compile_ao()
        input("press enter to continue.")
        run_ao(sys.argv[i+2:])

    elif x == "exec":
        run_ao(sys.argv[i+2:])
        break
