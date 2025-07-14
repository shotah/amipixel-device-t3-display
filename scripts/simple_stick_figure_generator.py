#!/usr/bin/env python3
"""
Simple Stick Figure GIF Generator for LVGL
Uses string-based pixel patterns for easy editing and visualization
"""

import os

# Animation definitions using string patterns
# '0' = transparent, '1' = filled pixel
ANIMATIONS = {
    'idle': [
        # Frame 1 - Standing (bigger, more visible)
        [
            "000000000110000000000000000000",
            "000000001001000000000000000000",
            "000000001001000000000000000000", 
            "000000000110000000000000000000",
            "000000001111100000000000000000",  # Mouth
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
        # Frame 2 - Eyes closed (blink)
        [
            "000000000110000000000000000000",
            "000000001111000000000000000000",  # Closed eyes
            "000000001111000000000000000000", 
            "000000000110000000000000000000",
            "000000001111100000000000000000",  # Mouth
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
    ],
    
    'happy': [
        # Frame 1 - Happy with arms up
        [
            "000000000110000000000000000000",
            "000000001001000000000000000000",
            "000000001001000000000000000000", 
            "000000000110000000000000000000",
            "000000001010100000000000000000",  # Smile
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000011100011100111000000000000",  # Arms up high
            "000001000011000010000000000000",
            "000000100011000100000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
        # Frame 2 - Jumping (feet together)
        [
            "000000000110000000000000000000",
            "000000001001000000000000000000",
            "000000001001000000000000000000", 
            "000000000110000000000000000000",
            "000000001010100000000000000000",  # Smile
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000011100011100111000000000000",  # Arms up high
            "000001000011000010000000000000",
            "000000100011000100000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000000000000000000000000",  # Feet off ground
            "000000000000000000000000000000",
            "000000000111100000000000000000",  # Feet together
            "000000000111100000000000000000",
            "000000000000000000000000000000",
        ],
    ],
    
    'eating': [
        # Frame 1 - Food approaching
        [
            "011000000110000000000000000000",  # Food (apple)
            "000000001001000000000000000000",
            "000000001001000000000000000000", 
            "000000000110000000000000000000",
            "000000001111100000000000000000",  # Mouth
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
        # Frame 2 - Food at mouth
        [
            "000000000110000000000000000000",
            "000000001001000000000000000000",
            "000000001001000000000000000000", 
            "000000110110000000000000000000",  # Food at mouth
            "000000001111100000000000000000",  # Mouth
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
        # Frame 3 - Eating (mouth open)
        [
            "000000000110000000000000000000",
            "000000001001000000000000000000",
            "000000001001000000000000000000", 
            "000000000110000000000000000000",
            "000000001111100000000000000000",  # Mouth open
            "000000000111100000000000000000",  # Mouth wider
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
    ],
    
    'sleeping': [
        # Frame 1 - Eyes closed
        [
            "000000000110000000000000000000",
            "000000001111000000000000000000",  # Closed eyes
            "000000001111000000000000000000", 
            "000000000110000000000000000000",
            "000000001111100000000000000000",  # Mouth
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
        # Frame 2 - With Z's (sleeping)
        [
            "000001100110000000000000000000",  # Z
            "000000110111000000000000000000",  # Z + closed eyes
            "000001100111000000000000000000",  # Z + closed eyes
            "000000000110000000000000000000",
            "000000001111100000000000000000",  # Mouth
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000001111011111000000000000000",  # Arms
            "000000000011000000000000000000",
            "000000000011000000000000000000",  # Body
            "000000000011000000000000000000",
            "000000000011000000000000000000",
            "000000001001000000000000000000",  # Legs
            "000000010000100000000000000000",
            "000000100000010000000000000000",
            "000001000000001000000000000000",
            "000010000000000100000000000000",
        ],
    ]
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

def generate_gif_files(animation_name, frames, width=30, height=20):
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
 * Pattern visualization:
'''
    
    # Add visual patterns as comments
    for i, frame in enumerate(frames):
        h_content += f' * Frame {i+1}:\n'
        for row in frame:
            h_content += f' * {row}\n'
        h_content += ' *\n'
    
    h_content += f''' */

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
        LV_IMG_CF_TRUE_COLOR, // cf
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
        print(f"     📐 Size: 30x20 pixels")
        print()
    
    print("🎉 All animations generated successfully!")
    print("\nTo use in your code:")
    print("  #include \"data/gifs/gif_idle.h\"")
    print("  lv_gif_set_src(img_avatar, &gif_idle);")
    print("\nAvailable animations:")
    for name in ANIMATIONS.keys():
        print(f"  - gif_{name}")

if __name__ == "__main__":
    main() 