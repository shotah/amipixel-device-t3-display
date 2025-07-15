#!/usr/bin/env python3
"""
Script to fix LVGL 8 to LVGL 9 API breaking changes in image files
"""

import os
import re
import glob

def fix_image_format(file_path):
    """Fix image format constants and structure in a single file"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Fix image format constants
        content = re.sub(r'LV_IMG_CF_TRUE_COLOR_ALPHA', 'LV_COLOR_FORMAT_NATIVE_WITH_ALPHA', content)
        content = re.sub(r'LV_IMG_CF_TRUE_COLOR', 'LV_COLOR_FORMAT_NATIVE', content)
        
        # Fix header structure fields that no longer exist
        # Remove always_zero and reserved fields
        content = re.sub(r'\.always_zero\s*=\s*\d+,?\s*\n', '', content)
        content = re.sub(r'\.reserved\s*=\s*\d+,?\s*\n', '', content)
        
        # Fix positional initialization format like in gif_sleeping.c
        # Pattern: LV_COLOR_FORMAT_NATIVE, // cf \n 0, // always_zero \n 0, // reserved \n
        pattern = r'LV_COLOR_FORMAT_NATIVE,\s*//\s*cf\s*\n\s*0,\s*//\s*always_zero\s*\n\s*0,\s*//\s*reserved\s*\n\s*(\d+),\s*//\s*w\s*\n\s*(\d+)\s*//\s*h'
        
        def replace_positional(match):
            w = match.group(1)
            h = match.group(2)
            return f'LV_COLOR_FORMAT_NATIVE, // cf\n        {w}, // w\n        {h} // h'
        
        content = re.sub(pattern, replace_positional, content)
        
        # Fix LV_COLOR_SIZE and LV_IMG_PX_SIZE_ALPHA_BYTE references
        # These constants no longer exist in LVGL 9
        content = re.sub(r'LV_COLOR_SIZE\s*/\s*8', '4', content)  # Assuming 32-bit color
        content = re.sub(r'LV_IMG_PX_SIZE_ALPHA_BYTE', '4', content)  # Assuming 4 bytes per pixel with alpha
        
        # Write back if changed
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"Fixed: {file_path}")
            return True
        else:
            print(f"No changes needed: {file_path}")
            return False
            
    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return False

def main():
    """Main function to fix all image files"""
    
    # Find all .c files in data directory
    patterns = [
        'src/data/gifs/*.c',
        'src/data/icons/*.c', 
        'src/data/images/*.c'
    ]
    
    files_fixed = 0
    total_files = 0
    
    for pattern in patterns:
        files = glob.glob(pattern)
        for file_path in files:
            total_files += 1
            if fix_image_format(file_path):
                files_fixed += 1
    
    print(f"\nSummary: Fixed {files_fixed} out of {total_files} files")

if __name__ == "__main__":
    main() 