import shutil, time, os

class config:
    src_path = "src"
    icon_path = "src/ico.o"
    includes = ["src/", "src/shared/"]
    defines = ["VERSION=1"]
    std = "c++20"
    optimization = "-O2"
    outpath = "bin/AO.exe"
    filedata = [
        ("src/aopch.h.gch", "g++ src/aopch.h"), # compile AO precompiled headers
        ("src/ico.o", "windres src/ico.rc -O coff -o src/ico.o") # create ico.o containing the data for AO icon
    ]

# convert seconds to hours, minutes and seconds
def sec_to_time(seconds):
    seconds = seconds % (24 * 3600)
    hour = seconds // 3600
    seconds %= 3600
    minutes = seconds // 60
    seconds %= 60

    hour = int(hour)
    minutes = int(minutes)
    seconds = int(seconds)

    t = [
        f"{hour} hour" if hour == 1 else f"{hour} hours" if hour > 0 else ""
        f"{minutes} minute" if minutes == 1 else f"{minutes} minutes" if minutes > 0 else ""
        f"{seconds} second" if seconds == 1 else f"{seconds} seconds" if seconds > 0 else ""
    ]

    print("total time: " + ", ".join(t))

def rm(*files):
    for i in files:
        if os.path.isdir(i):
            shutil.rmtree(i)

        elif os.path.isfile(i):
            os.remove(i)

def join(lst, prefix="", separator=" "):
    return separator.join([prefix + i for i in lst])

# https://stackoverflow.com/a/2909998/18121288
def get_src_paths(path, ext):
    return [os.path.join(path, f"*{ext}") for path, _, files in os.walk(path) if any(name.endswith(ext) for name in files)]

def precompile_files():
    for path, cmd in config.filedata:
        if not os.path.isfile(path):
            os.system(cmd)

def compile_ao():
    script = f"g++ {config.icon_path} {join(get_src_paths(config.src_path, "cpp"))} {join(config.includes, "-I")} {join(config.defines, "-D")} {config.optimization} -Wall -std={config.std} -o {config.outpath}"

    start = time.perf_counter()
    os.system(script)
    sec_to_time(time.perf_counter() - start)

def run_ao(args):
    os.system(f"{config.outpath} {join(args)}")
