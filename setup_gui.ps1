$imgui_url = "https://github.com/ocornut/imgui/archive/refs/heads/docking.zip"
$glfw_url = "https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip"

[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

# --- ImGui ---
Write-Host "Downloading ImGui..."
Invoke-WebRequest -Uri $imgui_url -OutFile "temp_imgui.zip"
Write-Host "Extracting ImGui..."
if (Test-Path "imgui-docking") { Remove-Item -Recurse -Force "imgui-docking" }
Expand-Archive -Path "temp_imgui.zip" -DestinationPath . -Force

Write-Host "Installing ImGui to include/imgui..."
if (Test-Path "include/imgui") { Remove-Item -Recurse -Force "include/imgui" }
Move-Item -Path "imgui-docking" -Destination "include/imgui"
Remove-Item "temp_imgui.zip"

# --- GLFW ---
Write-Host "Downloading GLFW..."
Invoke-WebRequest -Uri $glfw_url -OutFile "temp_glfw.zip"
Write-Host "Extracting GLFW..."
if (Test-Path "glfw-3.3.8.bin.WIN64") { Remove-Item -Recurse -Force "glfw-3.3.8.bin.WIN64" }
Expand-Archive -Path "temp_glfw.zip" -DestinationPath . -Force

Write-Host "Installing GLFW to include/glfw..."
if (Test-Path "include/glfw") { Remove-Item -Recurse -Force "include/glfw" }
mkdir "include/glfw" > $null
mkdir "include/glfw/include" > $null
mkdir "include/glfw/lib" > $null

# Move headers
Move-Item -Path "glfw-3.3.8.bin.WIN64/include/GLFW" -Destination "include/glfw/include"
# Move libs (supporting MinGW and VC)
# For MinGW (g++), we want lib-mingw-w64
Copy-Item -Path "glfw-3.3.8.bin.WIN64/lib-mingw-w64/*" -Destination "include/glfw/lib"
# We also might want VC libs if using MSVC.
Copy-Item -Path "glfw-3.3.8.bin.WIN64/lib-vc2022/*" -Destination "include/glfw/lib"

Remove-Item -Recurse -Force "glfw-3.3.8.bin.WIN64"
Remove-Item "temp_glfw.zip"

Write-Host "GUI Dependencies Installed."
