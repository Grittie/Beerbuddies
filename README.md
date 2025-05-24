# BeerBuddies

![initial sketch of BB](/assets/images/initial_sketch.png)

Beerbuddies is an interactive collectible toy project developed by a small team of designers and engineers with a shared passion for merging product design and electronics.

Our goal was to build a collectible mystery toy system that combines the tactile fun of collectible figures with embedded smart features. Beerbuddies figures aren’t just meant to sit on a shelf they come to life when placed on an interactive base station that plays sounds and recognizes each figure using NFC technology.

## Project structure
The repository is organized as follows:

```
├── assets/       # Photos and videos
├── docs/         # Project documentation
├── firmware/     # Embedded firmware source code (PlatformIO project)
│   ├── include/      # Project header files
│   ├── lib/          # Private project libraries
│   ├── src/          # Main firmware source code
│   ├── test/         # Unit and integration tests
│   ├── .vscode/      # VSCode configuration for PlatformIO
│   └── platformio.ini# PlatformIO configuration file
├── hardware/     # Hardware design files
│   ├── blender/      # Blender 3D models
│   ├── fusion/       # Fusion 360 CAD files
│   └── kicad/        # KiCad PCB design files
├── LICENSE       # Project license
├── README.md     # This file
```

## Technologies Used

- **ESP32 (Wemos D1 Mini ESP32)** for logic, NFC reading, and audio playback
- **PN532** or similar NFC module
- **MicroSD** or SPIFFS for storing audio files
- **I2S-compatible speaker driver**
- **LiPo battery with USB-C charging**
- **3D-printed injection mold-ready figures**
- **Blender / Fusion 360 / KiCad** for product and electronics design

##  Documentation
Follow the documentation [here](/docs/index.md).
