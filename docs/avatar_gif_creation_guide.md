# Avatar GIF Creation Guide for AmiPixel

## 🎯 **Quick Recommendation**

For your ESP32 digital pet project, I recommend **Piskel** (free) + **RunwayML** (AI-powered) combination:

1. **Piskel** for pixel-perfect, small file sizes
2. **RunwayML** for AI-generated base animations that you can pixel-ize

## 🤖 **AI GIF Generators**

### **Option 1: RunwayML (Best Quality)**

- **URL**: <https://runwayml.com/>
- **Cost**: ~$12/month (free trial available)
- **Prompts to try**:
  - "cute pixel art cat eating food, 8-bit style, looping animation"
  - "digital pet bunny sleeping with closed eyes, pixel art style"
  - "retro game character jumping, 16-bit animation"
  - "kawaii animal playing with ball, pixel art loop"

### **Option 2: Pika Labs**

- **URL**: <https://pika.art/>
- **Cost**: Free tier + paid plans
- **Best for**: Character consistency across animations

### **Option 3: Leonardo AI**

- **URL**: <https://leonardo.ai/>
- **Cost**: Free tier available
- **Features**: Motion feature for animating static images

## 🎨 **Pixel Art Tools (Recommended for ESP32)**

### **Option 1: Piskel (Free & Perfect)**

- **URL**: <https://www.piskelapp.com/>
- **Why perfect for ESP32**:
  - Creates tiny file sizes (perfect for 16MB flash)
  - Built-in GIF export
  - Pixel-perfect animations
  - Web-based (no installation)

**Quick Tutorial**:

1. Create new sprite (50x50 pixels)
2. Draw base avatar frame
3. Add frames for animation
4. Export as GIF
5. Use our converter script

### **Option 2: Aseprite (Professional)**

- **URL**: <https://www.aseprite.org/>
- **Cost**: $20 one-time
- **Features**: Professional pixel animation tools

## 📱 **Mobile Options**

### **DotPict (Mobile Pixel Art)**

- **iOS/Android**: Search "DotPict" in app stores
- **Features**: Create pixel art on phone/tablet
- **Perfect for**: Quick avatar creation on-the-go

### **8bit Painter (Mobile)**

- **iOS/Android**: Simple pixel art creation
- **Features**: Easy animation tools

## 🚀 **Recommended Workflow**

### **Phase 1: Base Avatar Creation**

1. **Use RunwayML** to generate base character concepts
   - Prompt: "cute digital pet avatar, pixel art style, front view, 50x50 pixels"
2. **Use Piskel** to create clean pixel art version
3. **Create base idle animation** (subtle breathing, blinking)

### **Phase 2: Action Animations**

For each action, create 4-8 frame loops:

#### **Eating Animation**

- Frame 1: Mouth closed, food nearby
- Frame 2: Mouth open
- Frame 3: Food partially eaten
- Frame 4: Mouth closed, happy expression
- **Piskel prompt**: Draw food getting smaller each frame

#### **Sleeping Animation**

- Frame 1: Eyes open
- Frame 2: Eyes half-closed
- Frame 3: Eyes closed
- Frame 4: "Zzz" symbols appear
- **AI prompt**: "pixel art character sleeping with Zzz symbols"

#### **Playing Animation**

- Frame 1: Normal pose
- Frame 2: Jumping up
- Frame 3: Peak jump
- Frame 4: Landing
- **AI prompt**: "pixel art character jumping playfully"

#### **Jumping Animation**

- Frame 1: Crouched down
- Frame 2: Mid-jump
- Frame 3: Peak height
- Frame 4: Landing

## 🎨 **Design Guidelines for ESP32**

### **Technical Constraints**

- **Size**: 50x50 pixels (optimal for T-Display-AMOLED)
- **Colors**: 8-16 colors max (for smaller file sizes)
- **Frames**: 4-8 frames per animation
- **Duration**: 1-3 seconds per loop
- **File Size**: Under 20KB each

### **Visual Style**

- **Pixel art style** (easier to compress)
- **Simple shapes** (less data)
- **Limited palette** (consistent look)
- **Clear expressions** (readable on small screen)

## 📊 **Health Stats Display Ideas**

### **Visual Health Bars**

```
Hunger:   [████████░░] 80%
Boredom:  [██████░░░░] 60%
Happiness:[██████████] 100%
```

### **Icon-Based Status**

- 🍎 Hunger level (apple fills/empties)
- 🎾 Boredom level (ball bounces more when bored)
- 😊 Happiness level (face expression changes)

## 🔧 **Implementation with Your Script**

Once you have GIFs created:

```bash
# Convert each GIF
python scripts/gif_converter.py eating_animation.gif gif_eating
python scripts/gif_converter.py sleeping_animation.gif gif_sleeping
python scripts/gif_converter.py playing_animation.gif gif_playing
python scripts/gif_converter.py jumping_animation.gif gif_jumping

# Add to ui_module.cpp
#include "data/gifs/gif_eating.c"
#include "data/gifs/gif_sleeping.c"
#include "data/gifs/gif_playing.c"
#include "data/gifs/gif_jumping.c"
```

## 🎯 **My Recommendation**

**Start with Piskel** (free) to prototype your avatar:

1. Create a simple 50x50 pixel base character
2. Make basic idle, eating, sleeping animations
3. Test on your ESP32
4. Once you like the concept, use AI tools to create more polished versions

**Character Ideas**:

- Pixel cat (classic digital pet)
- Tiny robot (tech theme)
- Blob creature (simple to animate)
- Pixel bunny (cute and expressive)

Would you like me to help you create the health stats display system or set up the additional buttons while you work on the GIF creation?
