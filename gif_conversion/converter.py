import os
from jinja2 import Environment, FileSystemLoader
from PIL import Image  # Import Pillow again

def gif_to_c_raw_bytes(input_gif_path, output_c_path, array_name, bytes_per_line=13):
    """
    Reads the entire GIF file as raw bytes and converts it to a C byte array.
    """
    try:
        with Image.open(input_gif_path) as img:  # Open image to get dimensions
            width = img.width
            height = img.height
    except FileNotFoundError:
        print(f"Error: GIF file not found at {input_gif_path}")
        return

    try:
        with open(input_gif_path, 'rb') as f:
            gif_data = f.read()
            print(f"First 10 bytes of gif_data: {gif_data[:10]}")  # Added print statement
    except FileNotFoundError:
        print(f"Error: GIF file not found at {input_gif_path}")
        return

    hex_data = [f"0x{byte:02x}" for byte in gif_data]

    template_data = {
        'array_name': array_name,
        'byte_data': hex_data,
        'data_size': len(gif_data),
        'input_gif_path': os.path.basename(input_gif_path),
        'bytes_per_line': bytes_per_line,
        'width': width,
        'height': height,
    }

    script_dir = os.path.dirname(os.path.abspath(__file__))
    env = Environment(loader=FileSystemLoader(script_dir))
    template = env.get_template('gif_to_c_template.jinja2')

    rendered_c_code = template.render(template_data)

    with open(output_c_path, 'w') as outfile:
        outfile.write(rendered_c_code)

    print(f"Successfully converted '{os.path.basename(input_gif_path)}' to raw byte C array '{array_name}' in '{os.path.basename(output_c_path)}' using template file.")

if __name__ == "__main__":
    script_dir = os.path.dirname(os.path.abspath(__file__))
    bytes_per_line = 13
    file_names = ["cat"]

    for name in file_names:
        input_gif_file = os.path.join(script_dir, f"{name}.gif")
        output_c_file = os.path.join(script_dir, f"gif_{name}.c")
        c_array_name = f"{name}"
        gif_to_c_raw_bytes(input_gif_file, output_c_file, c_array_name, bytes_per_line)