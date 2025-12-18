Import("env")
import shutil

def copy_firmware(source, target, env):
    firmware_src = str(target[0])
    # You can change the name here, or append a version number
    shutil.copyfile(firmware_src, "firmware_update.bin")
    print("Firmware copied to project root as firmware_update.bin")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", copy_firmware)