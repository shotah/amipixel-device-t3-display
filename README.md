# AmiPixel

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) **A friendly, interactive digital avatar device for social connection on the LilyGo T-Display S3 AMOLED Plus.**

![AmiPixel Avatar Example Image (Placeholder - Replace with an actual image later)](./assets/amipixel_avatar_example.png) ## Overview

AmiPixel is a project to create a charming and interactive digital avatar that lives on the LilyGo T-Display S3 AMOLED Plus. Inspired by classic digital pets like Tamagotchi and playful toys like Cube World, AmiPixel aims to be a delightful device for social connection with friends and family.

**Key Features (Planned - Subject to Development):**

* **Interactive Digital Avatar:** A visually engaging pixel art avatar that responds to user interaction and has its own simple needs and moods.
* **Core Care Actions:** Users can interact with AmiPixel through actions like "Feed," "Play," and "Pet" to care for their avatar.
* **Autonomous Actions:** AmiPixel will have its own set of autonomous actions and behaviors, making it feel alive and responsive even without direct interaction.
* **Basic Needs & Mood System:** A simple system to track AmiPixel's needs (e.g., hunger, boredom) and influence its mood and behavior.
* **Sleep Mode:** AmiPixel will have a sleep mode to conserve power and for nighttime.
* **Expandable Social Features (Future Goal):**  Planned expansion to enable AmiPixels on different devices to interact and connect over Wi-Fi, creating a social network of friendly avatars.
* **Customizable Settings (Future Goal):**  Settings page for device name, brightness, volume (if sound is added), etc.

## Inspirations

* **Cube World (Toy):** [Wikipedia - Cube World (toy)](https://en.wikipedia.org/wiki/Cube_World_(toy)) -  For its playful and interactive nature, and the idea of miniature worlds.
* **Tamagotchi:** [Wikipedia - Tamagotchi](https://en.wikipedia.org/wiki/Tamagotchi) - For the concept of a digital pet that needs care and attention, and its nostalgic appeal.
* **LilyGo T-Display S3 AMOLED Plus:** [LilyGo Product Page](https://lilygo.cc/products/t-display-s3-amoled?variant=43532279939253) - The hardware platform for AmiPixel, providing a vibrant AMOLED display and Wi-Fi connectivity.
* **LilyGo-AMOLED-Series Examples:** [Device GitHub Repo Examples](https://github.com/Xinyuan-LilyGO/LilyGo-AMOLED-Series/tree/master/examples) -  Leveraging existing examples and widgets for efficient development.

## Getting Started - Installation

Before you can build and upload AmiPixel, you need to install the PlatformIO Command-Line Interface (CLI).

**Recommended Method:  Using the VS Code PlatformIO IDE Extension**

1. **Install VS Code:** [https://code.visualstudio.com/](https://code.visualstudio.com/)
2. **Install the PlatformIO IDE extension** in VS Code.  Search for "PlatformIO IDE" in the Extensions Marketplace (Ctrl+Shift+X or Cmd+Shift+X) and install it.  The PlatformIO IDE extension will automatically install the PlatformIO CLI for you.
3. **Restart VS Code** after installing the extension.

**Alternative Method: Manual PlatformIO Core Installation**

If you prefer to install PlatformIO Core manually (without VS Code), follow the official installation guide: [https://docs.platformio.org/en/latest/core/installation.html](https://docs.platformio.org/en/latest/core/installation.html)

**Verify Installation**

After installation, open a terminal or command prompt and run:

```bash
pio --version
```

**Current Status:**  Project is in early development. Core avatar design and basic interactions are the initial focus.

## Contributing (Future - Open to Ideas)

Contributions are welcome!  Once the project is more established, we will define clear contribution guidelines.  For now, feel free to open issues for bug reports, feature requests, or to share your ideas!

## 1️⃣Support Product

| Product(PinMap)               | SOC        | Flash | PSRAM    | Resolution | Size      |
| ----------------------------- | ---------- | ----- | -------- | ---------- | --------- |
| [T-Display-AMOLED-Lite][1]    | ESP32-S3R8 | 16MB  | 8MB(OPI) | 194x368    | 1.47 Inch |
| [T-Display-S3 AMOLED][2]      | ESP32-S3R8 | 16MB  | 8MB(OPI) | 240x536    | 1.91 Inch |
| [T4-S3][3]                    | ESP32-S3R8 | 16MB  | 8MB(OPI) | 450x600    | 2.41 Inch |
| [T-Display-S3 AMOLED Plus][4] | ESP32-S3R8 | 16MB  | 8MB(OPI) | 240x536    | 1.91 Inch |

| Current consumption           | Working current             | sleep current           | sleep mode            |
| ----------------------------- | --------------------------- | ----------------------- | --------------------- |
| [T-Display-AMOLED-Lite][1]    | (240MHz) WiFi On 90~230+ mA | About 1.1mA             | timer wakeup          |
| [T-Display-S3 AMOLED][2]      | (240MHz) WiFi On 90~230+ mA | About 230uA             | external gpio0 wakeup |
| [T4-S3][3]                    | (240MHz) WiFi On 90~230+ mA | About 230uA             | external gpio0 wakeup |
| [T-Display-S3 AMOLED Plus][4] | (240MHz) WiFi On 90~230+ mA | About 230~700uA dynamic | external gpio0 wakeup |

[1]: https://www.lilygo.cc/products/t-display-amoled-lite
[2]: https://www.lilygo.cc/products/t-display-s3-amoled
[3]: https://www.lilygo.cc/products/t4-s3
[4]: https://www.lilygo.cc/products/t-display-s3-amoled

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contact

[Shotah](https://github.com/shotah)

---

**Let's build a friendly world, one pixel at a time!** ✨
