#!/usr/bin/env python3

import re
import sys

def fix_certification_file(file_path):
    """Fix LVGL 9 header structure issues in certification image files"""
    print(f"Processing {file_path}")
    
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        original_content = content
        
        # Fix tripled header pattern: .header .header .header.w -> .w
        content = re.sub(r'\.header\s+\.header\s+\.header\.w\s*=', '.w =', content)
        content = re.sub(r'\.header\s+\.header\s+\.header\.h\s*=', '.h =', content)
        content = re.sub(r'\.header\s+\.header\s+\.header\.cf\s*=', '.cf =', content)
        
        # Fix doubled header pattern: .header.header.w -> .w  
        content = re.sub(r'\.header\.header\.w\s*=', '.w =', content)
        content = re.sub(r'\.header\.header\.h\s*=', '.h =', content)
        content = re.sub(r'\.header\.header\.cf\s*=', '.cf =', content)
        
        # Remove legacy fields if they exist
        content = re.sub(r'\.always_zero\s*=\s*[^,\n]*[,\n]', '', content)
        content = re.sub(r'\.reserved\s*=\s*[^,\n]*[,\n]', '', content)
        
        # Clean up any extra whitespace/newlines created by removals
        content = re.sub(r'\n\s*\n\s*\n', '\n\n', content)
        
        if content != original_content:
            with open(file_path, 'w', encoding='utf-8') as f:
                f.write(content)
            print(f"✓ Fixed header structure in {file_path}")
            return True
        else:
            print(f"• No changes needed in {file_path}")
            return False
            
    except Exception as e:
        print(f"✗ Error processing {file_path}: {str(e)}")
        return False

def main():
    files_to_fix = [
        "src/data/images/img_certification_amoled_191_tp.c",
        "src/data/images/img_certification_t4_s3_241_tp.c"
    ]
    
    fixed_count = 0
    for file_path in files_to_fix:
        if fix_certification_file(file_path):
            fixed_count += 1
    
    print(f"\nFixed {fixed_count} certification image files")

if __name__ == "__main__":
    main() 