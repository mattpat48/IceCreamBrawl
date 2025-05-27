#!/bin/sh
# ______________________________________________________________________________
#
#  Compile raylib C++ project for Android - FIXED VERSION
# ______________________________________________________________________________

# stop on error and display each command as it gets executed. Optional step but helpful in catching where errors happen if they do.
set -xe

# NOTE: If you excluded any ABIs in the previous steps, remove them from this list too
ABIS="arm64-v8a armeabi-v7a x86 x86_64"

BUILD_TOOLS=android/sdk/build-tools/29.0.3
TOOLCHAIN=android/ndk/toolchains/llvm/prebuilt/linux-x86_64
NATIVE_APP_GLUE=android/ndk/sources/android/native_app_glue

# C++ specific flags - UPDATED
CXX_FLAGS="-std=c++17 -ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall \
	-Wformat -Werror=format-security -no-canonical-prefixes \
	-DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=33 \
	-D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L \
	-fno-exceptions"

# C flags for native app glue (still C code)
C_FLAGS="-ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall \
	-Wformat -Werror=format-security -no-canonical-prefixes \
	-DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=33 \
	-D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L"

INCLUDES="-I. -Isrc -Isrc/game -Isrc/engine -Isrc/screens \
	-Iraylib/src -Iraylib-cpp/include \
    -I$NATIVE_APP_GLUE \
    -I$TOOLCHAIN/sysroot/usr/include"

# Copy icons
if [ -f "assets/icon_ldpi.png" ]; then
    mkdir -p android/build/res/drawable-ldpi
    cp assets/icon_ldpi.png android/build/res/drawable-ldpi/icon.png
fi
if [ -f "assets/icon_mdpi.png" ]; then
    mkdir -p android/build/res/drawable-mdpi
    cp assets/icon_mdpi.png android/build/res/drawable-mdpi/icon.png
fi
if [ -f "assets/icon_hdpi.png" ]; then
    mkdir -p android/build/res/drawable-hdpi
    cp assets/icon_hdpi.png android/build/res/drawable-hdpi/icon.png
fi
if [ -f "assets/icon_xhdpi.png" ]; then
    mkdir -p android/build/res/drawable-xhdpi
    cp assets/icon_xhdpi.png android/build/res/drawable-xhdpi/icon.png
fi

# Copy other assets (resources from src/resources)
if [ -d "src/resources" ]; then
    mkdir -p android/build/assets
    cp -r src/resources/* android/build/assets/ 2>/dev/null || true
fi

# Create lib directories for each ABI
for ABI in $ABIS; do
    mkdir -p lib/$ABI
    mkdir -p android/build/lib/$ABI
done

# ______________________________________________________________________________
#
#  Compile
# ______________________________________________________________________________
#
for ABI in $ABIS; do
	case "$ABI" in
		"armeabi-v7a")
			CCTYPE="armv7a-linux-androideabi"
			ARCH="arm"
			LIBPATH="arm-linux-androideabi"
			ABI_FLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16"
			;;

		"arm64-v8a")
			CCTYPE="aarch64-linux-android"
			ARCH="aarch64"
			LIBPATH="aarch64-linux-android"
			ABI_FLAGS="-target aarch64 -mfix-cortex-a53-835769"
			;;

		"x86")
			CCTYPE="i686-linux-android"
			ARCH="i386"
			LIBPATH="i686-linux-android"
			ABI_FLAGS=""
			;;

		"x86_64")
			CCTYPE="x86_64-linux-android"
			ARCH="x86_64"
			LIBPATH="x86_64-linux-android"
			ABI_FLAGS=""
			;;
	esac
	
	# C compiler for native app glue
	CC="$TOOLCHAIN/bin/${CCTYPE}33-clang"
	# C++ compiler for project files
	CXX="$TOOLCHAIN/bin/${CCTYPE}33-clang++"

	echo "Building for ABI: $ABI with $CXX"

	# Compile native app glue (C code)
	# .c -> .o
	$CC -c $NATIVE_APP_GLUE/android_native_app_glue.c -o $NATIVE_APP_GLUE/native_app_glue_$ABI.o \
		$INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $C_FLAGS -std=c99 $ABI_FLAGS

	# .o -> .a
	$TOOLCHAIN/bin/llvm-ar rcs lib/$ABI/libnative_app_glue.a $NATIVE_APP_GLUE/native_app_glue_$ABI.o

	# Clean previous object files
	find src -name "*.o" -delete 2>/dev/null || true

	# Compile all C++ source files
	echo "Compiling C++ sources for $ABI..."
	
	# Find and compile all .cpp files
	find src -name "*.cpp" -type f | while read cppfile; do
		echo "Compiling: $cppfile"
		$CXX -c "$cppfile" -o "$cppfile.o" \
			$INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $CXX_FLAGS $ABI_FLAGS
	done
	
	# Find and compile any .c files (if present)
	find src -name "*.c" -type f | while read cfile; do
		echo "Compiling: $cfile"
		$CC -c "$cfile" -o "$cfile.o" \
			$INCLUDES -I$TOOLCHAIN/sysroot/usr/include/$CCTYPE $C_FLAGS -std=c99 $ABI_FLAGS
	done

	# Collect all object files
	OBJECT_FILES=$(find src -name "*.o" -type f | tr '\n' ' ')
	
	echo "Linking for $ABI..."
	echo "Object files: $OBJECT_FILES"

	# FIXED LINKING COMMAND - Updated library order and paths
	$TOOLCHAIN/bin/ld.lld $OBJECT_FILES -o android/build/lib/$ABI/libmain.so -shared \
		--exclude-libs libatomic.a --build-id \
		-z noexecstack -z relro -z now \
		--warn-shared-textrel --fatal-warnings -u ANativeActivity_onCreate \
		-L$TOOLCHAIN/sysroot/usr/lib/$LIBPATH/33 \
		-L$TOOLCHAIN/sysroot/usr/include \
		-L$TOOLCHAIN/sysroot/usr/include/$LIBATH \
		-L$TOOLCHAIN/lib/clang/19/lib/linux/$ARCH \
		-L$TOOLCHAIN/sysroot/usr/lib/$LIBPATH \
		-L. -Landroid/build/obj -Llib/$ABI \
		-lraylib -lraylib-cpp -lnative_app_glue -llog -landroid -lEGL -lGLESv2 -lOpenSLES \
		-latomic -lc -lm -ldl -lstdc++ -lc++abi -lc++_static

	echo "Completed build for $ABI"
done

# ______________________________________________________________________________
#
#  Build APK
# ______________________________________________________________________________
#
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
# NOTE: If you changed the storepass and keypass in the setup process, change them here too
$BUILD_TOOLS/zipalign -f 4 game.apk game.final.apk
mv -f game.final.apk game.apk

# Install apksigner with `sudo apt install apksigner`
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

# Clean object files
echo "Cleaning temporary files..."
find src -name "*.o" -delete 2>/dev/null || true

echo "Android build process completed successfully!"