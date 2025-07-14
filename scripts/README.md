# AmiPixel GIF Conversion Scripts

This directory contains tools to help convert GIF files to C format for use with LVGL in the AmiPixel project.

## Files

- `gif_converter.py` - Main conversion script
- `README.md` - This file

## Quick Start

### Prerequisites

1. **Node.js** - Required for LVGL converter

   ```bash
   # Check if installed
   node --version
   
   # Install if needed (example for Ubuntu)
   sudo apt update
   sudo apt install nodejs npm
   ```

2. **Git** - Required to download LVGL converter

   ```bash
   git --version
   ```

### Converting a GIF

```bash
# Navigate to the scripts directory
cd scripts

# Convert a GIF file
python gif_converter.py path/to/your/animation.gif gif_sleep

# This will create: src/data/gifs/gif_sleep.c
```

### Using the Converted GIF

1. **Include in your source code:**

   ```cpp
   #include "data/gifs/gif_sleep.c"
   ```

2. **Declare the GIF:**

   ```cpp
   LV_IMG_DECLARE(gif_sleep);
   ```

3. **Use in LVGL:**

   ```cpp
   lv_obj_t *gif_obj = lv_gif_create(parent);
   lv_gif_set_src(gif_obj, &gif_sleep);
   ```

## Examples

### Creating a Sleep Animation

```bash
python gif_converter.py sleeping_cat.gif gif_sleep
```

### Creating Happy/Sad States

```bash
python gif_converter.py happy_animation.gif gif_happy
python gif_converter.py sad_animation.gif gif_sad
```

## Tips for Good GIF Conversions

1. **Size**: Keep GIFs small (50x50 pixels or similar) for ESP32 memory constraints
2. **Colors**: Use limited color palettes for better compression
3. **Duration**: Short loops (1-3 seconds) work best for avatar animations
4. **File Size**: Aim for under 20KB per GIF to fit in ESP32 flash memory

## Troubleshooting

### "Node.js not found"

Install Node.js from <https://nodejs.org/>

### "Git not found"

Install Git from <https://git-scm.com/>

### "Conversion failed"

- Check that the input file is a valid GIF
- Ensure the output name is a valid C identifier (no spaces, starts with letter)
- Try a smaller or simpler GIF

### "Out of memory" errors

- Reduce GIF dimensions
- Use fewer colors
- Shorten animation length

## Manual Conversion (Alternative)

If the script doesn't work, you can use LVGL's online converter:

1. Go to <https://lvgl.github.io/lv_img_conv/>
2. Upload your GIF
3. Select "C array" output format
4. Select "CF_TRUE_COLOR" color format
5. Download the .c file
6. Move it to `src/data/gifs/`
7. Include it in your project

## Format Consistency

All GIFs should use the same format for consistency:

- **Color Format**: `CF_TRUE_COLOR`
- **Output Format**: C array
- **Naming**: `gif_<state>` (e.g., `gif_sleep`, `gif_happy`)

This ensures all animations work properly with LVGL's GIF system.
