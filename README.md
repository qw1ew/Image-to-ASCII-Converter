# ASCII Converter 🎨

A powerful, high-performance tool to convert images into ASCII art. Designed with a **Cyberpunk-styled GUI** for ease of use and a **Command Line Interface (CLI)** for batch processing.

![Banner](https://via.placeholder.com/800x300?text=ASCII+Converter+Pro+Demo)

## 🌟 Key Features

*   **Dual Mode Architecture**:
    *   **GUI Mode**: Modern, dark-themed graphical interface with live preview.
    *   **CLI Mode**: Efficient command-line tool for scripting and automation.
*   **Broad Format Support**: Opens **JPG, PNG, BMP, TGA**, and even **WebP**.
*   **Smart Platform Optimization**:
    *   **Discord**: Automatically scales art to fit 2000-character limits and wraps it in code blocks.
    *   **Telegram**: Optimized width for mobile readability.
    *   **Console**: Standard width for terminal outputs.
*   **Advanced Styling**:
    *   **High Detail**: Uses complex character sets for rich shading.
    *   **Block Fill**: "Minecraft-style" dense character mapping.
    *   **Invert Color**: Perfect for light-background apps.
    *   **True Color**: (CLI) Supports full ANSI 24-bit color output.

---

## 🚀 How to Build & Run (The Easy Way)

### Prerequisites
You only need a C++ compiler installed on your Windows machine:
*   **Option A**: [Visual Studio](https://visualstudio.microsoft.com/) (Free Community Edition with C++ workload).
*   **Option B**: [MinGW-w64](https://www.mingw-w64.org/) (gcc/g++).

### One-Click Setup
1.  **Download the project**.
2.  **Run the Setup Scripts** (Windows PowerShell):
    *   Right-click `setup_gui.ps1` -> "Run with PowerShell" (Downloads ImGui & GLFW).
    *   Right-click `setup_webp.ps1` -> "Run with PowerShell" (Downloads libwebp).
3.  **Build**:
    *   Double-click **`build.bat`**.
    *   Wait for the "Build successful!" message.

🎉 **That's it!** You now have `ascii_converter.exe`.

---

## 🎮 Usage 

### 1. Graphical Mode (GUI)
Simply double-click `ascii_converter.exe` **without** any arguments.

*   **Drag & Drop**: Pull any image info the window.
*   **Controls on the Left**:
    *   **Platform**: Select "Discord" to auto-magically fix size limits.
    *   **Resolution**: Slider to adjust width (10-300 chars).
    *   **Style**: Toggle "High Detail" or "Invert".
*   **Export**:
    *   **Save to File**: Exports `.txt`.
    *   **Copy to Clipboard**: Copies formatted text ready to paste.

### 2. Command Line Mode (CLI)
Run via CMD or PowerShell for manual control.

**Syntax:**
```bash
ascii_converter.exe <path_to_image> [width] [flags]
```

**Examples:**
```bash
# Basic conversion (Width 100)
ascii_converter.exe image.jpg 100

# High detail, no color
ascii_converter.exe logo.png 150 --complex --no-color

# WebP support works natively
ascii_converter.exe sticker.webp 80
```

---

## 🛠️ Tech Stack
*   **Language**: C++17
*   **GUI**: [Dear ImGui](https://github.com/ocornut/imgui) (Docking Branch) + [GLFW](https://www.glfw.org/)
*   **Image Processing**: [stb_image](https://github.com/nothings/stb) & [libwebp](https://github.com/webmproject/libwebp)

## 📄 License
MIT License. Free to use and modify.
