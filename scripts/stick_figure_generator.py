#!/usr/bin/env python3
"""
Stick Figure GIF Generator for AmiPixel
=====================================

This script creates stick figure animations using pure code - no external images needed!
Define your stick figure using 0s and 1s, then generate animated GIFs programmatically.

Features:
- Pure bitmap patterns (0s and 1s)
- Programmable animations
- Memory-efficient for ESP32
- No external dependencies
- Full control over every pixel

Usage:
    python stick_figure_generator.py
"""

import os
import struct
from pathlib import Path

class StickFigureGenerator:
    def __init__(self, width=50, height=50):
        self.width = width
        self.height = height
        self.project_root = Path(__file__).parent.parent
        self.output_dir = self.project_root / "src" / "data" / "gifs"
        self.output_dir.mkdir(parents=True, exist_ok=True)
        
    def create_basic_stick_figure(self):
        """Create a basic stick figure pattern"""
        # 50x50 grid for our stick figure
        pattern = [[0 for _ in range(self.width)] for _ in range(self.height)]
        
        # Head (circle-ish)
        head_center_x, head_center_y = 25, 12
        head_radius = 6
        for y in range(max(0, head_center_y - head_radius), min(self.height, head_center_y + head_radius + 1)):
            for x in range(max(0, head_center_x - head_radius), min(self.width, head_center_x + head_radius + 1)):
                if (x - head_center_x)**2 + (y - head_center_y)**2 <= head_radius**2:
                    pattern[y][x] = 1
        
        # Body (vertical line)
        body_x = 25
        for y in range(18, 35):
            pattern[y][body_x] = 1
            
        # Arms (horizontal line)
        arm_y = 24
        for x in range(15, 36):
            pattern[arm_y][x] = 1
            
        # Legs (diagonal lines)
        leg_start_x = 25
        leg_start_y = 35
        
        # Left leg
        for i in range(12):
            x = leg_start_x - i//2
            y = leg_start_y + i
            if 0 <= x < self.width and 0 <= y < self.height:
                pattern[y][x] = 1
                
        # Right leg
        for i in range(12):
            x = leg_start_x + i//2
            y = leg_start_y + i
            if 0 <= x < self.width and 0 <= y < self.height:
                pattern[y][x] = 1
        
        return pattern
    
    def create_eating_animation(self):
        """Create eating animation frames"""
        frames = []
        
        # Base stick figure
        base_pattern = self.create_basic_stick_figure()
        
        # Frame 1: Normal stick figure
        frame1 = [row[:] for row in base_pattern]
        frames.append(frame1)
        
        # Frame 2: Food appears near mouth
        frame2 = [row[:] for row in base_pattern]
        # Add food (small square) near the head
        for y in range(10, 12):
            for x in range(32, 34):
                if 0 <= x < self.width and 0 <= y < self.height:
                    frame2[y][x] = 1
        frames.append(frame2)
        
        # Frame 3: Food closer to mouth
        frame3 = [row[:] for row in base_pattern]
        for y in range(11, 13):
            for x in range(30, 32):
                if 0 <= x < self.width and 0 <= y < self.height:
                    frame3[y][x] = 1
        frames.append(frame3)
        
        # Frame 4: Food in mouth (smaller)
        frame4 = [row[:] for row in base_pattern]
        frame4[12][28] = 1  # Small food piece
        frames.append(frame4)
        
        return frames
    
    def create_sleeping_animation(self):
        """Create sleeping animation frames"""
        frames = []
        
        # Base stick figure
        base_pattern = self.create_basic_stick_figure()
        
        # Frame 1: Eyes open (normal)
        frame1 = [row[:] for row in base_pattern]
        # Add eyes
        frame1[10][23] = 1  # Left eye
        frame1[10][27] = 1  # Right eye
        frames.append(frame1)
        
        # Frame 2: Eyes half closed
        frame2 = [row[:] for row in base_pattern]
        # Smaller eyes
        frame2[11][23] = 1  # Left eye (lower)
        frame2[11][27] = 1  # Right eye (lower)
        frames.append(frame2)
        
        # Frame 3: Eyes closed
        frame3 = [row[:] for row in base_pattern]
        # No eyes, just sleeping lines
        frame3[10][22] = 1  # Left eye line
        frame3[10][23] = 1
        frame3[10][24] = 1
        frame3[10][26] = 1  # Right eye line
        frame3[10][27] = 1
        frame3[10][28] = 1
        frames.append(frame3)
        
        # Frame 4: Add "Zzz"
        frame4 = [row[:] for row in base_pattern]
        # Keep closed eyes
        frame4[10][22] = 1
        frame4[10][23] = 1
        frame4[10][24] = 1
        frame4[10][26] = 1
        frame4[10][27] = 1
        frame4[10][28] = 1
        
        # Add "Zzz" pattern
        # First Z
        for x in range(30, 33):
            frame4[5][x] = 1    # Top line
            frame4[7][x] = 1    # Bottom line
        frame4[6][32] = 1       # Diagonal
        frame4[6][31] = 1
        
        # Second Z (smaller)
        for x in range(34, 36):
            frame4[3][x] = 1    # Top line
            frame4[5][x] = 1    # Bottom line
        frame4[4][35] = 1       # Diagonal
        
        frames.append(frame4)
        
        return frames
    
    def create_happy_animation(self):
        """Create happy bouncing animation"""
        frames = []
        
        # Create 4 frames of bouncing motion
        for bounce_offset in [0, -2, -4, -2]:  # Bounce up and down
            pattern = [[0 for _ in range(self.width)] for _ in range(self.height)]
            
            # Adjust all Y coordinates by bounce_offset
            # Head
            head_center_x, head_center_y = 25, 12 + bounce_offset
            head_radius = 6
            for y in range(max(0, head_center_y - head_radius), min(self.height, head_center_y + head_radius + 1)):
                for x in range(max(0, head_center_x - head_radius), min(self.width, head_center_x + head_radius + 1)):
                    if (x - head_center_x)**2 + (y - head_center_y)**2 <= head_radius**2:
                        pattern[y][x] = 1
            
            # Happy eyes
            if 0 <= 10 + bounce_offset < self.height:
                pattern[10 + bounce_offset][23] = 1  # Left eye
                pattern[10 + bounce_offset][27] = 1  # Right eye
            
            # Smile
            if 0 <= 14 + bounce_offset < self.height:
                for x in range(23, 28):
                    pattern[14 + bounce_offset][x] = 1
                pattern[13 + bounce_offset][24] = 1
                pattern[13 + bounce_offset][26] = 1
            
            # Body
            body_x = 25
            for y in range(max(0, 18 + bounce_offset), min(self.height, 35 + bounce_offset)):
                pattern[y][body_x] = 1
                
            # Arms (raised up for happy)
            arm_y = 24 + bounce_offset
            if 0 <= arm_y < self.height:
                # Left arm up
                for i in range(8):
                    x = 25 - i
                    y = arm_y - i//2
                    if 0 <= x < self.width and 0 <= y < self.height:
                        pattern[y][x] = 1
                # Right arm up
                for i in range(8):
                    x = 25 + i
                    y = arm_y - i//2
                    if 0 <= x < self.width and 0 <= y < self.height:
                        pattern[y][x] = 1
            
            # Legs
            leg_start_x = 25
            leg_start_y = 35 + bounce_offset
            
            # Left leg
            for i in range(12):
                x = leg_start_x - i//2
                y = leg_start_y + i
                if 0 <= x < self.width and 0 <= y < self.height:
                    pattern[y][x] = 1
                    
            # Right leg
            for i in range(12):
                x = leg_start_x + i//2
                y = leg_start_y + i
                if 0 <= x < self.width and 0 <= y < self.height:
                    pattern[y][x] = 1
            
            frames.append(pattern)
        
        return frames
    
    def create_idle_animation(self):
        """Create subtle idle animation (blinking)"""
        frames = []
        
        # Base stick figure
        base_pattern = self.create_basic_stick_figure()
        
        # Frame 1-3: Eyes open
        frame_open = [row[:] for row in base_pattern]
        # Add eyes
        frame_open[10][23] = 1  # Left eye
        frame_open[10][27] = 1  # Right eye
        
        for _ in range(3):
            frames.append([row[:] for row in frame_open])
        
        # Frame 4: Eyes closed (blink)
        frame_blink = [row[:] for row in base_pattern]
        # Closed eyes (lines)
        frame_blink[10][22] = 1
        frame_blink[10][23] = 1
        frame_blink[10][24] = 1
        frame_blink[10][26] = 1
        frame_blink[10][27] = 1
        frame_blink[10][28] = 1
        frames.append(frame_blink)
        
        return frames
    
    def pattern_to_gif_data(self, frames, gif_name):
        """Convert bitmap patterns to GIF format data"""
        # This creates a simplified GIF structure
        # For a real implementation, you'd need proper GIF encoding
        # For now, we'll create a format that works with LVGL
        
        gif_data = []
        
        # Simple color palette: 0 = transparent, 1 = black
        palette = [
            (0, 0, 0),      # Black for pixels
            (255, 255, 255), # White for background
        ]
        
        # Convert each frame to binary data
        for frame in frames:
            frame_data = []
            for row in frame:
                for pixel in row:
                    # Convert to RGB565 format (common for embedded displays)
                    if pixel == 1:
                        frame_data.append(0x00)  # Black pixel
                        frame_data.append(0x00)
                    else:
                        frame_data.append(0xFF)  # White pixel
                        frame_data.append(0xFF)
            gif_data.extend(frame_data)
        
        return gif_data
    
    def generate_c_file(self, frames, gif_name):
        """Generate C file with stick figure GIF data"""
        gif_data = self.pattern_to_gif_data(frames, gif_name)
        
        output_file = self.output_dir / f"{gif_name}.c"
        
        c_content = f'''#ifdef __has_include
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

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_GIF_{gif_name.upper()}
#define LV_ATTRIBUTE_IMG_GIF_{gif_name.upper()}
#endif

/*
 * Procedurally generated stick figure animation
 * Created using pure code - no external images!
 * 
 * Animation: {gif_name}
 * Size: {self.width}x{self.height}
 * Frames: {len(frames)}
 */

// clang-format off
const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_GIF_{gif_name.upper()} uint8_t {gif_name}_map[] = {{
'''
        
        # Add the bitmap data
        for i, byte in enumerate(gif_data):
            if i % 16 == 0:
                c_content += "\n    "
            c_content += f"0x{byte:02x}, "
        
        c_content += f'''
}};
// clang-format on

const lv_img_dsc_t {gif_name} = {{
    .header = {{
        .cf = LV_IMG_CF_TRUE_COLOR,
        .always_zero = 0,
        .reserved = 0,
        .w = {self.width},
        .h = {self.height},
    }},
    .data_size = sizeof({gif_name}_map),
    .data = {gif_name}_map,
}};

/*
 * Stick Figure Pattern (for reference):
 * 
 * Frame 1 pattern:
'''
        
        # Add visual representation of first frame
        if frames:
            for row in frames[0]:
                c_content += " * "
                for pixel in row:
                    c_content += "#" if pixel == 1 else "."
                c_content += "\n"
        
        c_content += " */\n"
        
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(c_content)
        
        print(f"✅ Generated {output_file}")
        return output_file
    
    def generate_all_animations(self):
        """Generate all stick figure animations"""
        animations = {
            'gif_cat': self.create_idle_animation(),
            'gif_rabbit': self.create_happy_animation(),
            'gif_sleep': self.create_sleeping_animation(),
            'gif_eating': self.create_eating_animation(),
        }
        
        generated_files = []
        for name, frames in animations.items():
            file_path = self.generate_c_file(frames, name)
            generated_files.append(file_path)
        
        return generated_files
    
    def print_pattern(self, pattern):
        """Print a visual representation of the pattern"""
        for row in pattern:
            line = ""
            for pixel in row:
                line += "#" if pixel == 1 else "."
            print(line)

def main():
    print("🚀 AmiPixel Stick Figure Generator")
    print("=" * 50)
    
    generator = StickFigureGenerator()
    
    print("Creating stick figure animations...")
    
    # Generate all animations
    files = generator.generate_all_animations()
    
    print(f"\n✅ Generated {len(files)} stick figure animations!")
    print("\nFiles created:")
    for file_path in files:
        print(f"  📁 {file_path}")
    
    print("\n🎯 Next steps:")
    print("1. Build your project: make build")
    print("2. Upload to device: make deployfs")
    print("3. Watch your stick figure avatar come to life!")
    print("\n💡 To modify animations, edit the pattern functions in this script")
    print("   and run it again to regenerate the GIF files.")

if __name__ == "__main__":
    main() 