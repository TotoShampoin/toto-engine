import sys
import os

# 1. scan recursively all files in src/embed folder
# 2. convert all txt and binary files to .CPP std::array

def convert_file(file_path, dest_folder):
    print(file_path)
    file_name = os.path.basename(file_path)
    var_name = file_name.replace(".", "_")
    var_name = var_name.replace("-", "_")
    var_name = var_name.upper()
    var_name = "EMBED_" + var_name

    with open(file_path, "rb") as f:
        data = f.read()

    with open(os.path.join(dest_folder, file_name + ".cpp"), "w") as f:
        f.write("#include <array>\n")
        f.write("#include <stdint.h>\n")
        f.write("const std::array<char, %d> %s = {\n" % (len(data), var_name))
        for i in range(0, len(data), 16):
            f.write("    ")
            for j in range(16):
                if i + j < len(data):
                    f.write("0x%02X, " % data[i+j])
            f.write("\n")
        f.write("};\n")

def scan_and_convert(folder, dest_folder):
    working_dir = os.getcwd()
    for root, dirs, files in os.walk(folder):
        for file in files:
            if(not file.endswith(".cpp")):
                convert_file(os.path.join(root, file), dest_folder)

if __name__ == "__main__":
    scan_and_convert("embed", "src/embed")
