#!/usr/bin/env python3
"""
Simple Stick Figure GIF Generator for LVGL
Uses string-based pixel patterns for easy editing and visualization
"""

import os
from frames.base_frames import BASE_FRAME
from frames.idle_frames import IDLE_FRAMES
from frames.happy_frames import HAPPY_FRAMES
from frames.sleeping_frames import SLEEPING_FRAMES

# Animation definitions using imported frame data
# All frames are exactly 60x40 (60 characters wide, 40 rows tall)
ANIMATIONS = {
    'idle': IDLE_FRAMES,
    'happy': HAPPY_FRAMES,
    'sleeping': SLEEPING_FRAMES
}

def pattern_to_c_array(pattern, width, height):
    """Convert string pattern to C array data"""
    c_data = []
    
    for row in pattern:
        # Pad or truncate row to exact width
        row = row.ljust(width, '0')[:width]
        
        # Convert to pixels (0=transparent, 1=black stick figure)
        for char in row:
            if char == '1':
                # Black pixel (RGB565: 0x0000) - black stick figure
                c_data.extend([0x00, 0x00])
            else:
                # Transparent pixel (RGB565: 0xFFFF) - white/transparent background
                c_data.extend([0xFF, 0xFF])
    
    # Pad to exact size if needed
    target_size = width * height * 2  # 2 bytes per pixel (RGB565)
    while len(c_data) < target_size:
        c_data.extend([0x00, 0x00])
    
    return c_data[:target_size]

def generate_gif_files(animation_name, frames, width=60, height=40):
    """Generate LVGL C and H files for an animation"""
    
    # Calculate dimensions and frame data
    total_frames = len(frames)
    frame_size = width * height * 2  # 2 bytes per pixel (RGB565)
    
    # Generate frame data
    all_frame_data = []
    for frame in frames:
        frame_data = pattern_to_c_array(frame, width, height)
        all_frame_data.extend(frame_data)
    
    # Create header file content
    h_content = f'''#pragma once

#ifdef __has_include
#if __has_include("lvgl.h")
#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#define LV_LVGL_H_INCLUDE_SIMPLE
#endif
#endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifdef __cplusplus
extern "C" {{
#endif

/*
 * Simple stick figure animation: {animation_name}
 * Generated from string patterns - easy to edit!
 * Size: {width}x{height} pixels
 * Frames: {total_frames}
 * 
 * Frame 0 is always the base frame for consistency.
 */

// External declaration of the GIF data
extern const lv_img_dsc_t gif_{animation_name};

#ifdef __cplusplus
}}
#endif
'''
    
    # Create C file content
    c_content = f'''#include "gif_{animation_name}.h"

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_GIF_{animation_name.upper()}
#define LV_ATTRIBUTE_IMG_GIF_{animation_name.upper()}
#endif

// Image data array
const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_IMG_GIF_{animation_name.upper()} uint8_t gif_{animation_name}_data[] = {{
'''
    
    # Add data in chunks of 16 bytes per line
    for i in range(0, len(all_frame_data), 16):
        chunk = all_frame_data[i:i+16]
        hex_values = ', '.join(f'0x{b:02x}' for b in chunk)
        c_content += f'    {hex_values},\n'
    
    c_content += '};\n\n'
    
    # Add descriptor using C-style initialization (compatible with C++)
    c_content += f'''const lv_img_dsc_t gif_{animation_name} = {{
    {{ // header
        LV_COLOR_FORMAT_NATIVE, // cf
        0, // always_zero
        0, // reserved
        {width}, // w
        {height} // h
    }},
    {len(all_frame_data)}, // data_size
    gif_{animation_name}_data // data
}};
'''
    
    return h_content, c_content

def main():
    """Generate all stick figure animations"""
    
    # Create output directory
    output_dir = "../src/data/gifs"
    os.makedirs(output_dir, exist_ok=True)
    
    print("🎨 Generating simple stick figure animations...")
    print(f"Output directory: {output_dir}")
    print("📂 Frame data imported from separate files:")
    print("   - frames/base_frames.py (BASE_FRAME)")
    print("   - frames/idle_frames.py (IDLE_FRAMES)")
    print("   - frames/happy_frames.py (HAPPY_FRAMES)")
    print("   - frames/sleeping_frames.py (SLEEPING_FRAMES)")
    print()
    
    # Generate each animation
    for name, frames in ANIMATIONS.items():
        print(f"  📝 Generating {name}...")
        
        # Generate C and H content
        h_content, c_content = generate_gif_files(name, frames)
        
        # Write header file
        h_output_file = os.path.join(output_dir, f"gif_{name}.h")
        with open(h_output_file, 'w') as f:
            f.write(h_content)
        
        # Write C file
        c_output_file = os.path.join(output_dir, f"gif_{name}.c")
        with open(c_output_file, 'w') as f:
            f.write(c_content)
        
        print(f"     ✅ Created: {h_output_file}")
        print(f"     ✅ Created: {c_output_file}")
        print(f"     📊 Frames: {len(frames)}")
        print(f"     📐 Size: 60x40 pixels")
        print()
    
    print("🎉 All animations generated successfully!")
    print("\n📁 Frame organization:")
    print("  ├── scripts/frames/")
    print("  │   ├── __init__.py")
    print("  │   ├── base_frames.py      (BASE_FRAME - neutral standing)")
    print("  │   ├── idle_frames.py      (IDLE_FRAMES - 2 frames)")
    print("  │   ├── happy_frames.py     (HAPPY_FRAMES - 5 frames)")
    print("  │   └── sleeping_frames.py  (SLEEPING_FRAMES - 6 frames)")
    print("  └── simple_stick_figure_generator.py (main script)")
    print("\n🚀 To add more frames:")
    print("  1. Edit the appropriate frames/*.py file")
    print("  2. Add new frame patterns as lists of 40 strings (60 chars each)")
    print("  3. Run this script to regenerate the GIF files")
    print("\nTo use in your code:")
    print("  #include \"data/gifs/gif_idle.h\"")
    print("  lv_img_set_src(img_avatar, &gif_idle);")
    print("\nAvailable animations:")
    for name in ANIMATIONS.keys():
        print(f"  - gif_{name}")
    print("\nKey improvements:")
    print("  ✨ Thicker lines (2-3 pixels) for better visibility")
    print("  🛌 Proper sleeping animation: lying down → sleeping with Z's → waking up")
    print("  📐 All frames standardized to 60x40 pixels")
    print("  🎯 Frame 0 is always the same base position for consistency")
    print("  📂 Organized frame data in separate files for easy editing")

if __name__ == "__main__":
    main() 