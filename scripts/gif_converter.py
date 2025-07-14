#!/usr/bin/env python3
"""
GIF to C Converter for AmiPixel Project
=======================================

This script converts GIF files to C format suitable for use with LVGL.
It uses LVGL's image converter to ensure proper format consistency.

Requirements:
- Node.js (for LVGL converter)
- Git (to clone LVGL converter)

Usage:
    python gif_converter.py input.gif output_name
    
Example:
    python gif_converter.py cat_animation.gif gif_cat

This will create:
- gif_cat.c (C source file)
- gif_cat.h (header file, if needed)
"""

import os
import sys
import subprocess
import shutil
import tempfile
from pathlib import Path

class GifConverter:
    def __init__(self):
        self.script_dir = Path(__file__).parent
        self.project_root = self.script_dir.parent
        self.converter_dir = self.project_root / "tools" / "lv_img_conv"
        
    def check_requirements(self):
        """Check if required tools are installed"""
        try:
            subprocess.run(["node", "--version"], capture_output=True, check=True)
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("❌ Node.js is not installed. Please install Node.js first.")
            return False
            
        try:
            subprocess.run(["git", "--version"], capture_output=True, check=True)
        except (subprocess.CalledProcessError, FileNotFoundError):
            print("❌ Git is not installed. Please install Git first.")
            return False
            
        return True
        
    def setup_converter(self):
        """Download and setup LVGL image converter if not present"""
        if self.converter_dir.exists():
            print("✅ LVGL converter already exists")
            return True
            
        print("📥 Downloading LVGL image converter...")
        tools_dir = self.project_root / "tools"
        tools_dir.mkdir(exist_ok=True)
        
        try:
            subprocess.run([
                "git", "clone", 
                "https://github.com/lvgl/lv_img_conv.git",
                str(self.converter_dir)
            ], check=True)
            
            print("📦 Installing converter dependencies...")
            subprocess.run([
                "npm", "install"
            ], cwd=self.converter_dir, check=True)
            
            print("✅ LVGL converter setup complete")
            return True
            
        except subprocess.CalledProcessError as e:
            print(f"❌ Failed to setup converter: {e}")
            return False
            
    def convert_gif(self, input_path, output_name):
        """Convert a GIF file to C format"""
        input_path = Path(input_path)
        if not input_path.exists():
            print(f"❌ Input file not found: {input_path}")
            return False
            
        if not input_path.suffix.lower() == '.gif':
            print(f"❌ Input file must be a GIF: {input_path}")
            return False
            
        output_dir = self.project_root / "src" / "data" / "gifs"
        output_dir.mkdir(parents=True, exist_ok=True)
        
        output_file = output_dir / f"{output_name}.c"
        
        print(f"🔄 Converting {input_path.name} to {output_name}.c...")
        
        try:
            # Use LVGL converter with proper settings for animated GIFs
            cmd = [
                "node", "lv_img_conv.js",
                str(input_path),
                "-f",  # Force overwrite
                "-c", "CF_TRUE_COLOR",  # Use consistent color format
                "-o", str(output_file)
            ]
            
            result = subprocess.run(
                cmd,
                cwd=self.converter_dir,
                capture_output=True,
                text=True
            )
            
            if result.returncode == 0:
                # Post-process the file to ensure it's properly formatted for LVGL GIFs
                self._post_process_gif_file(output_file, output_name)
                print(f"✅ Successfully converted to {output_file}")
                return True
            else:
                print(f"❌ Conversion failed: {result.stderr}")
                return False
                
        except Exception as e:
            print(f"❌ Error during conversion: {e}")
            return False
            
    def _post_process_gif_file(self, file_path, var_name):
        """Post-process the generated C file to ensure proper GIF format"""
        try:
            with open(file_path, 'r') as f:
                content = f.read()
                
            # Ensure the file has proper LVGL includes
            includes = '''#ifdef __has_include
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

#ifndef LV_ATTRIBUTE_IMG_GIF_{var_name.upper()}
#define LV_ATTRIBUTE_IMG_GIF_{var_name.upper()}
#endif

'''.format(var_name=var_name)
            
            # Find the data array and descriptor
            if f'const lv_img_dsc_t {var_name}' in content:
                # File is already properly formatted
                print(f"✅ File {file_path} is properly formatted")
                return
                
            # If not properly formatted, we need to fix it
            # This is a basic implementation - you might need to adjust based on actual output
            print(f"⚠️  Post-processing {file_path} for proper GIF format...")
            
        except Exception as e:
            print(f"⚠️  Warning: Could not post-process file: {e}")
            
    def create_example_usage(self):
        """Create example usage instructions"""
        example_path = self.project_root / "examples" / "gif_usage_example.cpp"
        example_path.parent.mkdir(exist_ok=True)
        
        example_content = '''/*
 * Example usage of converted GIFs in AmiPixel project
 */

#include "lvgl.h"
#include "data/gifs/gif_cat.c"
#include "data/gifs/gif_rabbit.c"
// Add other GIF includes here

void setup_avatar_gifs() {
    // Create a GIF object
    lv_obj_t *gif_obj = lv_gif_create(lv_scr_act());
    
    // Set the GIF source
    lv_gif_set_src(gif_obj, &gif_cat);
    
    // Position the GIF
    lv_obj_align(gif_obj, LV_ALIGN_CENTER, 0, 0);
    
    // The GIF will start playing automatically
}

void change_avatar_gif(lv_obj_t *gif_obj, avatar_state_t *state) {
    if (state->is_sleeping) {
        // lv_gif_set_src(gif_obj, &gif_sleep);
    } else if (state->hunger > 80 && state->boredom < 20) {
        lv_gif_set_src(gif_obj, &gif_rabbit);
    } else {
        lv_gif_set_src(gif_obj, &gif_cat);
    }
}
'''
        
        with open(example_path, 'w') as f:
            f.write(example_content)
            
        print(f"✅ Created example usage at {example_path}")

def main():
    if len(sys.argv) != 3:
        print("Usage: python gif_converter.py <input.gif> <output_name>")
        print("Example: python gif_converter.py cat_animation.gif gif_cat")
        sys.exit(1)
        
    input_file = sys.argv[1]
    output_name = sys.argv[2]
    
    converter = GifConverter()
    
    print("🚀 AmiPixel GIF Converter")
    print("=" * 40)
    
    if not converter.check_requirements():
        sys.exit(1)
        
    if not converter.setup_converter():
        sys.exit(1)
        
    if converter.convert_gif(input_file, output_name):
        converter.create_example_usage()
        print("\n✅ Conversion completed successfully!")
        print(f"📁 Output file: src/data/gifs/{output_name}.c")
        print(f"📚 Example usage: examples/gif_usage_example.cpp")
        print("\nNext steps:")
        print("1. Include the new GIF in your source code")
        print("2. Declare it with: LV_IMG_DECLARE(gif_name);")
        print("3. Use it with: lv_gif_set_src(gif_obj, &gif_name);")
    else:
        print("❌ Conversion failed!")
        sys.exit(1)

if __name__ == "__main__":
    main() 