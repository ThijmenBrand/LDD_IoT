Import("env")
import shutil

def copy_firmware(source, target, env):
    firmware_src = str(target[0])
    out_name = "firmware.bin"
    shutil.copyfile(firmware_src, out_name)

    firmware_txt = "firmware.txt"
    try:
        with open(firmware_txt, "r") as f:
            cur = int(f.read().strip())
    except Exception:
        cur = 0
    new = cur + 1
    with open(firmware_txt, "w") as f:
        f.write(str(new))

    print("Firmware copied to project root as firmware_update.bin")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_firmware)