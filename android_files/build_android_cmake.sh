#!/bin/bash
# Build script per Android usando CMake
set -e

# Configurazione percorsi
ANDROID_SDK_PATH="android/sdk"
ANDROID_NDK_PATH="android/ndk"
BUILD_TOOLS="$ANDROID_SDK_PATH/build-tools/29.0.3"

# ABIs da compilare
ABIS="arm64-v8a armeabi-v7a x86 x86_64"

echo "=== Building Android project with CMake ==="

# Crea directory build se non esiste
mkdir -p build/android

# Copia assets e icone
echo "Copying assets and icons..."
cp assets/icon_ldpi.png android/build/res/drawable-ldpi/icon.png
cp assets/icon_mdpi.png android/build/res/drawable-mdpi/icon.png
cp assets/icon_hdpi.png android/build/res/drawable-hdpi/icon.png
cp assets/icon_xhdpi.png android/build/res/drawable-xhdpi/icon.png
cp assets/* android/build/assets

# Compila per ogni ABI
for ABI in $ABIS; do
    echo "=== Building for ABI: $ABI ==="
    
    BUILD_DIR="build/android/$ABI"
    mkdir -p "$BUILD_DIR"
    
    # Configura CMake per Android
    cmake -S . -B "$BUILD_DIR" \
        -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_PATH/build/cmake/android.toolchain.cmake" \
        -DANDROID_ABI="$ABI" \
        -DANDROID_PLATFORM=android-29 \
        -DANDROID_NDK="$ANDROID_NDK_PATH" \
        -DCMAKE_BUILD_TYPE=Release \
        -DANDROID_ARM_NEON=ON \
        -DANDROID_STL=c++_shared
    
    # Compila
    cmake --build "$BUILD_DIR" --parallel $(nproc)
    
    # Copia la libreria compilata nella directory giusta
    mkdir -p "android/build/lib/$ABI"
    cp "$BUILD_DIR/libmain.so" "android/build/lib/$ABI/"
    
    echo "Completed build for $ABI"
done

echo "=== All ABIs compiled successfully! ==="

# ______________________________________________________________________________
#
#  Build APK (questa parte rimane uguale al tuo script originale)
# ______________________________________________________________________________

echo "Building APK..."

# Ensure build directories exist
mkdir -p android/build/src/com/raylib/game
mkdir -p android/build/dex
mkdir -p android/build/obj

$BUILD_TOOLS/aapt package -f -m \
    -S android/build/res -J android/build/src -M android/build/AndroidManifest.xml \
    -I android/sdk/platforms/android-29/android.jar

# Compile NativeLoader.java
if [ -f "android/build/src/com/raylib/game/NativeLoader.java" ]; then
    javac -verbose -source 1.8 -target 1.8 -d android/build/obj \
        -bootclasspath jre/lib/rt.jar \
        -classpath android/sdk/platforms/android-29/android.jar:android/build/obj \
        -sourcepath src android/build/src/com/raylib/game/NativeLoader.java
fi

$BUILD_TOOLS/dx --verbose --dex --output=android/build/dex/classes.dex android/build/obj

# Add resources and assets to APK
$BUILD_TOOLS/aapt package -f \
    -M android/build/AndroidManifest.xml -S android/build/res -A android/build/assets \
    -I android/sdk/platforms/android-29/android.jar -F game.apk android/build/dex

# Add libraries to APK
cd android/build
for ABI in $ABIS; do
    if [ -f "lib/$ABI/libmain.so" ]; then
        ../../$BUILD_TOOLS/aapt add ../../game.apk lib/$ABI/libmain.so
    fi
done
cd ../..

# Zipalign APK and sign
echo "Signing APK..."
$BUILD_TOOLS/zipalign -f 4 game.apk game.final.apk
mv -f game.final.apk game.apk

# Install apksigner
if command -v apksigner > /dev/null; then
    apksigner sign --ks android/raylib.keystore --out my-app-release.apk --ks-pass pass:raylib game.apk
    mv my-app-release.apk game.apk
else
    echo "Warning: apksigner not found. APK not signed."
fi

echo "APK build completed: game.apk"

# Install to device or emulator (optional)
if command -v adb > /dev/null; then
    echo "Installing APK to device/emulator..."
    android/sdk/platform-tools/adb install -r game.apk
else
    echo "ADB not found. Skipping installation."
fi

echo "Android build process completed successfully!"