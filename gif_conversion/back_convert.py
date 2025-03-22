import re
import os

def c_array_to_gif(c_file_path, output_gif_path, array_name):
    """
    Extracts a byte array from a C file and saves it as a GIF.
    """
    try:
        with open(c_file_path, 'r') as f:
            c_code = f.read()
    except FileNotFoundError:
        print(f"Error: C file not found at {c_file_path}")
        return

    # Use a regular expression to find the byte array content
    re_string = r"uint8_t\s+" + re.escape(array_name) + r"\[\s*\]\s*=\s*\{\s*(.*?)\s*\};"
    match = re.search(re_string, c_code, re.DOTALL)
    if not match:
        print(f"Error: Byte array '{array_name}' not found in the C file.")
        return

    byte_str = match.group(1)
    hex_values = re.findall(r"0x[0-9a-fA-F]{2}", byte_str)
    byte_values = [int(hex_val, 16) for hex_val in hex_values]

    if not byte_values:
        print("Error: No valid byte values found in the array.")
        return

    try:
        with open(output_gif_path, 'wb') as f:
            f.write(bytes(byte_values))
        print(f"Successfully saved the byte array to '{output_gif_path}'")
    except Exception as e:
        print(f"Error saving GIF file: {e}")

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))

    file_names = ["rabbit"]
    file_names = ["cat"]
    for name in file_names:
        c_file_path = os.path.join(script_dir, f"gif_{name}.c")
        output_gif_path = os.path.join(script_dir, f"{name}_converted_back.gif")
        # array_name = f"gif_{name}_map"
        array_name = f"{name}_gif_map"
        c_array_to_gif(c_file_path, output_gif_path, array_name)
    