# pip install Pillow
# pip install Jinja2
from PIL import Image
from jinja2 import Environment, FileSystemLoader
import os

def gif_to_c_array(input_gif_path, output_c_path, array_name, bytes_per_line=13):
    """
    Converts an animated GIF to a C byte array using a Jinja2 template file with configurable bytes per line.

    Make sure to install dependencies:
    pip install Pillow
    pip install Jinja2

    Args:
        input_gif_path (str): Path to the input GIF file.
        output_c_path (str): Path to the output C file.
        array_name (str): Name of the C array to create.
        bytes_per_line (int): Number of byte values to write per line in the C file.
    """
    try:
        img = Image.open(input_gif_path)
    except FileNotFoundError:
        print(f"Error: GIF file not found at {input_gif_path}")
        return

    frames_data = []
    frame_durations = []
    width = img.width
    height = img.height

    try:
        while True:
            frame_data = img.convert("RGB").tobytes()
            frames_data.append(frame_data)
            frame_durations.append(img.info.get('duration', 100))  # Default duration is 100ms

            img.seek(img.tell() + 1)
    except EOFError:
        # End of GIF frames
        pass

    total_data_hex = [f"0x{byte:02x}" for byte in b"".join(frames_data)]

    template_data = {
        'array_name': array_name,
        'byte_data': total_data_hex,
        'frame_count': len(frames_data),
        'frame_durations': frame_durations,
        'input_gif_path': os.path.basename(input_gif_path),
        'width': width,
        'height': height,
        'bytes_per_line': bytes_per_line
    }

    # Create a Jinja2 environment and load the template from the script's directory
    script_dir = os.path.dirname(os.path.abspath(__file__))
    env = Environment(loader=FileSystemLoader(script_dir))
    template = env.get_template('gif_to_c_template.jinja2')

    rendered_c_code = template.render(template_data)

    with open(output_c_path, 'w') as outfile:
        outfile.write(rendered_c_code)

    print(f"Successfully converted '{os.path.basename(input_gif_path)}' to C array '{array_name}' in '{os.path.basename(output_c_path)}' using template file.")

if __name__ == "__main__":
    # pip install Pillow
    # pip install Jinja2
    script_dir = os.path.dirname(os.path.abspath(__file__))
    bytes_per_line = 13
    file_names = ["falling_over"]

    for name in file_names:
        # Falling Over Animation
        input_gif_file = os.path.join(script_dir, f"{name}.gif")
        output_c_file = os.path.join(script_dir, f"{name}_anim.c")
        c_array_name = f"{name}"
        gif_to_c_array(input_gif_file, output_c_file, c_array_name, bytes_per_line)
