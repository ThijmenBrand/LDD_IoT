Import("env")
import shutil
import os

version_file = "firmware.txt"
current_version = 0

if os.path.exists(version_file):
    try:
        with open(version_file, "r") as f:
            current_version = int(f.read().strip())
    except Exception:
        pass

new_version = current_version + 1

env.Append(CPPDEFINES=[("CURRENT_VERSION", new_version)])    

def copy_firmware(source, target, env):
    firmware_src = str(target[0])
    out_name = "firmware.bin"
    shutil.copyfile(firmware_src, out_name)

    with open(version_file, "w") as f:
        f.write(str(new_version))
    print(f"firmware.txt updated to version {new_version}")

    print("Firmware copied to project root as firmware_update.bin")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_firmware)