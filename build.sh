#!/bin/bash

# ICB Game Engine - Unified Build Script
# Usage: ./build.sh [desktop|android|clean|help]

set -e  # Exit on error

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_ROOT/build"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_help() {
    echo -e "${BLUE}ICB Game Engine - Build Script${NC}"
    echo ""
    echo "Usage: $0 [COMMAND]"
    echo ""
    echo "Commands:"
    echo "  desktop    Build for desktop (Linux/Windows/macOS)"
    echo "  android    Build Android APK"
    echo "  clean      Clean all build files"
    echo "  help       Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 desktop          # Build desktop version"
    echo "  $0 android          # Build Android APK"
    echo "  $0 clean            # Clean all builds"
}

log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

check_dependencies() {
    log_info "Checking dependencies..."
    
    if ! command -v cmake &> /dev/null; then
        log_error "CMake not found. Please install CMake 3.24 or higher."
        exit 1
    fi
    
    CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
    log_info "Found CMake $CMAKE_VERSION"
}

build_desktop() {
    log_info "Building desktop version..."
    
    check_dependencies
    
    # Create build directory
    mkdir -p "$BUILD_DIR/desktop"
    cd "$BUILD_DIR/desktop"
    
    # Configure
    log_info "Configuring CMake..."
    cmake "$PROJECT_ROOT" \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
    
    # Build
    log_info "Building project..."
    cmake --build . --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    log_success "Desktop build completed!"
    log_info "Executable: $BUILD_DIR/desktop/ICB_game"
    if [ -d "$BUILD_DIR/desktop/resources" ]; then
        log_info "Resources: $BUILD_DIR/desktop/resources"
    fi
}

build_android() {
    log_info "Building Android APK..."
    
    # Check Android dependencies
    if [ ! -d "$PROJECT_ROOT/android/ndk" ] || [ ! -d "$PROJECT_ROOT/android/sdk" ]; then
        log_error "Android NDK/SDK not found in android/ directory"
        log_error "Please ensure android/ndk and android/sdk directories exist"
        exit 1
    fi
    
    log_info "Ensuring CMake dependencies are downloaded..."
    mkdir -p "$BUILD_DIR/android_deps"
    cd "$BUILD_DIR/android_deps"
    cmake "$PROJECT_ROOT" -DCMAKE_SYSTEM_NAME=Android || true
    cd "$PROJECT_ROOT"

    export RAYLIB_SRC_DIR="$BUILD_DIR/android_deps/_deps/raylib-src"
    export RAYLIB_CPP_SRC_DIR="$BUILD_DIR/android_deps/_deps/raylib-cpp-src"

    # Make sure build script is executable
    chmod +x "$PROJECT_ROOT/build_android.sh"
    
    # Run Android build script
    cd "$PROJECT_ROOT"
    ./build_android.sh
    
    log_success "Android build completed!"
    if [ -f "$PROJECT_ROOT/game.apk" ]; then
        log_info "APK created: $PROJECT_ROOT/game.apk"
    fi
}

clean_builds() {
    log_info "Cleaning build files..."
    
    # Clean CMake builds
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        log_info "Removed $BUILD_DIR"
    fi
    
    # Clean Android builds
    if [ -d "$PROJECT_ROOT/android/build" ]; then
        rm -rf "$PROJECT_ROOT/android/build"
        log_info "Removed android/build"
    fi
    
    if [ -f "$PROJECT_ROOT/game.apk" ]; then
        rm "$PROJECT_ROOT/game.apk"
        log_info "Removed game.apk"
    fi
    
    # Clean any leftover object files
    find "$PROJECT_ROOT" -name "*.o" -delete 2>/dev/null || true
    
    log_success "Clean completed!"
}

# Main script logic
case "${1:-desktop}" in
    "desktop")
        build_desktop
        ;;
    "android")
        build_android
        ;;
    "clean")
        clean_builds
        ;;
    "help"|"-h"|"--help")
        print_help
        ;;
    *)
        log_error "Unknown command: $1"
        print_help
        exit 1
        ;;
esac