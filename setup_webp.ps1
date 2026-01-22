$url = "https://github.com/webmproject/libwebp/archive/refs/tags/v1.3.2.zip"
$output = "temp_libwebp.zip"

Write-Host "Downloading libwebp..."
try {
    [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
    Invoke-WebRequest -Uri $url -OutFile $output
} catch {
    Write-Error "Download failed: $_"
    exit 1
}

Write-Host "Extracting..."
try {
    if (Test-Path "libwebp-1.3.2") { Remove-Item -Recurse -Force "libwebp-1.3.2" }
    Expand-Archive -Path $output -DestinationPath . -Force
} catch {
    Write-Error "Extraction failed: $_"
    exit 1
}

Write-Host "Moving to include/libwebp..."
try {
    if (Test-Path "include/libwebp") { Remove-Item -Recurse -Force "include/libwebp" }
    Move-Item -Path "libwebp-1.3.2" -Destination "include/libwebp"
} catch {
    Write-Error "Move failed: $_"
    exit 1
}

Write-Host "Done."
