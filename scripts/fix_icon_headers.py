#!/usr/bin/env python3
"""
Script to fix duplicated header structure issues in LVGL icon files
"""

import os
import re
import glob

def fix_icon_headers(file_path):
    """Fix duplicated header structure in icon files"""
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        
        original_content = content
        
        # Fix pattern like:
        # .header.cf = LV_COLOR_FORMAT_NATIVE_WITH_ALPHA,
        # .header    .header    .header.w = 64,
        # .header.h = 64,
        
        # Pattern to match the problematic structure
        pattern = r'\.header\.cf\s*=\s*(LV_COLOR_FORMAT_\w+),\s*\n\s*\.header\s*\.header\s*\.header\.w\s*=\s*(\d+),\s*\n\s*\.header\.h\s*=\s*(\d+),'
        
        def replace_header(match):
            cf = match.group(1)
            w = match.group(2)
            h = match.group(3)
            return f'.header = {{\n        .cf = {cf},\n        .w = {w},\n        .h = {h}\n    }},'
        
        content = re.sub(pattern, replace_header, content)
        
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
    """Main function to fix all icon files"""
    
    # Find all .c files in icons directory
    pattern = 'src/data/icons/*.c'
    
    files_fixed = 0
    total_files = 0
    
    files = glob.glob(pattern)
    for file_path in files:
        total_files += 1
        if fix_icon_headers(file_path):
            files_fixed += 1
    
    print(f"\nSummary: Fixed {files_fixed} out of {total_files} icon files")

if __name__ == "__main__":
    main() 