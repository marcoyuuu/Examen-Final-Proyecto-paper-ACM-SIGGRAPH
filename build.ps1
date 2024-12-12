<#
.SYNOPSIS
    Automated build script for OpenGL project.
.DESCRIPTION
    Handles build process with optional cleanup, configuration, and build logging.
.PARAMETER Clean
    Cleans the build directory before starting the build.
.PARAMETER Generator
    Specifies the CMake generator (default: "MinGW Makefiles").
.PARAMETER BuildType
    Specifies the build type (default: "Debug").
.PARAMETER BuildDir
    Specifies the build directory (default: "./build").
#>

param(
    [switch]$Clean,
    [string]$Generator = "MinGW Makefiles",
    [string]$BuildType = "Debug",
    [string]$BuildDir = "./build"
)

# Enable strict error handling
$ErrorActionPreference = "Stop"

# Define paths
$ScriptDir = (Get-Location).Path
$BuildDir = Join-Path $ScriptDir ($BuildDir -replace "^\./", "")
$SourceDir = $ScriptDir
$LogFile = Join-Path $BuildDir "build.log"

# Logging function
function Log {
    param([string]$Message)
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    Write-Host "$Timestamp - $Message"
    Add-Content -Path $LogFile -Value "$Timestamp - $Message"
}

# Clean build directory
function CleanBuildDir {
    if (Test-Path $BuildDir) {
        Log "Cleaning build directory: $BuildDir"
        Remove-Item -Recurse -Force $BuildDir
    }
}

# Verify Asset Directories
function VerifyAssets {
    $ShadersDir = Join-Path $SourceDir "assets/shaders"
    $TexturesDir = Join-Path $SourceDir "assets/textures"
    
    if (-not (Test-Path $ShadersDir)) {
        throw "Shaders directory not found: $ShadersDir"
    }
    
    if (-not (Test-Path $TexturesDir)) {
        throw "Textures directory not found: $TexturesDir"
    }
    
    Log "Verified existence of shaders and textures directories."
}

try {
    # Ensure required tools are available
    if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
        throw "CMake is not installed or not in PATH."
    }
    if (-not (Get-Command mingw32-make -ErrorAction SilentlyContinue)) {
        throw "MinGW Make (mingw32-make) is not installed or not in PATH."
    }

    # Verify asset directories
    VerifyAssets

    # Clean if requested
    if ($Clean) {
        CleanBuildDir
    }

    # Create build directory
    if (-not (Test-Path $BuildDir)) {
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
        Log "Created build directory: $BuildDir"
    }

    # Create or append to log file
    if (-not (Test-Path $LogFile)) {
        New-Item -Path $LogFile -ItemType File | Out-Null
        Log "Created log file: $LogFile"
    }

    # Navigate to build directory
    Set-Location $BuildDir

    # Define compiler paths
    # Adjust these paths based on your MinGW installation
    $CCompiler = "C:/msys64/mingw64/bin/gcc.exe"
    $CXXCompiler = "C:/msys64/mingw64/bin/g++.exe"

    if (-not (Test-Path $CCompiler)) {
        throw "C compiler not found at path: $CCompiler"
    }

    if (-not (Test-Path $CXXCompiler)) {
        throw "C++ compiler not found at path: $CXXCompiler"
    }

    # Run CMake with output directories and explicit compilers
    Log "Configuring project with CMake..."
    cmake -G "$Generator" `
          -DCMAKE_BUILD_TYPE=$BuildType `
          -DCMAKE_RUNTIME_OUTPUT_DIRECTORY="$BuildDir/bin" `
          -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="$BuildDir/bin" `
          -DCMAKE_C_COMPILER="$CCompiler" `
          -DCMAKE_CXX_COMPILER="$CXXCompiler" `
          -S "$SourceDir" `
          -B "$BuildDir" `
          2>&1 | Tee-Object -Variable CMakeOutput | Out-Host
    if ($LASTEXITCODE -ne 0) {
        throw "CMake configuration failed."
    }

    # Run build
    Log "Building project with Make..."
    mingw32-make 2>&1 | Tee-Object -Variable MakeOutput | Out-Host
    if ($LASTEXITCODE -ne 0) {
        throw "Build process failed."
    }

    # Log success
    Log "Build process completed successfully."

    # Return to script directory
    Set-Location $ScriptDir

} catch {
    Log "ERROR: $_"
    Exit 1
}
