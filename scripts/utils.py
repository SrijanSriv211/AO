import hashlib, shutil, time, os

CONFIG = {
    "SRC_PATH": "src",
    "ICON_PATH": "src/ico.o",
    "INCLUDES": ["src/", "src/shared/"],
    "DEFINES": ["VERSION=1"],
    "STD": "c++20",
    "OPTIMIZATION": "-O2",
    "OUTPATH": "bin/AO.exe",
    "PRECOMPILES": [
        ("src/aopch.h.gch", "g++ src/aopch.h"), # compile AO precompiled headers
        ("src/ico.o", "windres src/ico.rc -O coff -o src/ico.o") # create ico.o containing the data for AO icon
    ]
}

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

    total_time = ", ".join(t)
    total_time = total_time if total_time else "0"

    print("total time: " + total_time)

def mkdirs(*dirs):
    [os.mkdir(dir) for dir in dirs if not os.path.isdir(dir)]

def rm(files):
    for i in files:
        if os.path.isdir(i):
            shutil.rmtree(i)

        elif os.path.isfile(i):
            os.remove(i)

def join(lst, prefix="", separator=" "):
    return separator.join([prefix + i for i in lst])

def precompile_files():
    for path, cmd in CONFIG["PRECOMPILES"]:
        if not os.path.isfile(path):
            os.system(cmd)

def get_files(path: str, ext):
    src = []

    for path, _, files in os.walk(path):
        src.extend([os.path.join(path, file) for file in files if file.endswith(ext)])

    return src

def create_unique_name(filepath: str):
    return filepath.replace("/", "_").replace("\\", "_").replace(":", "_")

def hash_file(file):
    md5_h = hashlib.md5()

    with open(file, "rb") as f:
        for chunk in iter(lambda: f.read(4096), b""):
            md5_h.update(chunk)

    return md5_h.hexdigest()

def save_hash(file, hash_value):
    unique_name = create_unique_name(file)
    hash_file = os.path.join("obj", unique_name + ".hash")

    with open(hash_file, 'w') as f:
        f.write(hash_value)

def load_hash(file):
    unique_name = create_unique_name(file)
    hash_file = os.path.join("obj", unique_name + ".hash")

    if os.path.exists(hash_file):
        with open(hash_file, 'r') as f:
            return f.read().strip()

    return None

def delete_stale_objects():
    valid_hashes = {create_unique_name(f) + ".hash" for f in get_files(CONFIG["SRC_PATH"], "cpp")}
    obj_hashes = {i.replace("obj\\", "") for i in get_files("obj", "hash")}

    for i in list(obj_hashes - valid_hashes):
        obj_name = i.replace(".hash", ".o")
        rm(["obj\\" + i, "obj\\" + obj_name])

################################################################################################################################
###################################################### CODE TO COMPILE AO ######################################################
################################################################################################################################

def COMMON():
    return f"{join(CONFIG["INCLUDES"], "-I")} {join(CONFIG["DEFINES"], "-D")} {CONFIG["OPTIMIZATION"]} -std={CONFIG["STD"]}"

def compile_to_obj(file):
    unique_name = create_unique_name(file)
    obj_file = os.path.join("obj", unique_name + ".o")
    current_hash = hash_file(file)
    previous_hash = load_hash(file)

    if current_hash != previous_hash or not os.path.isfile(obj_file):
        os.system(f"g++ -c {file} {COMMON()} -o {obj_file}")
        save_hash(file, current_hash)

    return obj_file

def compile_ao():
    start = time.perf_counter()

    mkdirs("bin", "obj")
    delete_stale_objects()
    obj_files = [compile_to_obj(f) for f in get_files(CONFIG["SRC_PATH"], "cpp")]
    os.system(f"g++ {CONFIG["ICON_PATH"]} {join(obj_files)} {COMMON()} -Wall -o {CONFIG["OUTPATH"]}")

    sec_to_time(time.perf_counter() - start)

def run_ao(args):
    os.system(f"{CONFIG["OUTPATH"]} {join(args)}")
