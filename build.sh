#!/bin/sh
# ______________________________________________________________________________
#
#  Compile raylib project for Android (C++ version)
# ______________________________________________________________________________

set -xe

ABIS="arm64-v8a armeabi-v7a x86 x86_64"

BUILD_TOOLS=android/sdk/build-tools/29.0.3
TOOLCHAIN=android/ndk/toolchains/llvm/prebuilt/linux-x86_64
NATIVE_APP_GLUE=android/ndk/sources/android/native_app_glue

FLAGS="-ffunction-sections -funwind-tables -fstack-protector-strong -fPIC -Wall \
	-Wformat -Werror=format-security -no-canonical-prefixes \
	-DANDROID -DPLATFORM_ANDROID -D__ANDROID_API__=29"

MY_SRC_PATHS=$(find src -type d | sed 's/^/-I /')

# Unisci tutto nella variabile INCLUDES
INCLUDES="-I. -Iinclude -I../include \
          -I$NATIVE_APP_GLUE \
          -Iraylib/src \
          -Iraylib-cpp/include \
          $MY_SRC_PATHS"

# Copy icons
cp assets/icon_ldpi.png android/build/res/drawable-ldpi/icon.png
cp assets/icon_mdpi.png android/build/res/drawable-mdpi/icon.png
cp assets/icon_hdpi.png android/build/res/drawable-hdpi/icon.png
cp assets/icon_xhdpi.png android/build/res/drawable-xhdpi/icon.png

# Copy other assets
cp assets/* android/build/assets

# ______________________________________________________________________________
#
#  Compile
# ______________________________________________________________________________
#       
for ABI in $ABIS; do
    # 1. RESET E ASSEGNAZIONE VARIABILI (Fondamentale!)
    case "$ABI" in
        "arm64-v8a")
            CCTYPE="aarch64-linux-android"
            ARCH_DIR="aarch64-linux-android"
            LIBPATH="aarch64-linux-android"
            ARCH="aarch64"
            TARGET="aarch64-linux-android29"
            ABI_FLAGS="-march=armv8-a -std=c++17 -fexceptions -frtti"
            ;;
        "armeabi-v7a")
            CCTYPE="armv7a-linux-androideabi"
            ARCH_DIR="arm-linux-androideabi"
            LIBPATH="arm-linux-androideabi"
            ARCH="arm"
            TARGET="armv7a-linux-androideabi29"
            ABI_FLAGS="-march=armv7-a -mfloat-abi=softfp -mfpu=vfpv3-d16 -std=c++17 -fexceptions -frtti"
            ;;
        "x86")
            TARGET="i686-linux-android29"
            ARCH_DIR="i686-linux-android"
            LIBPATH="i686-linux-android"
            # PROVA A CAMBIARE QUESTA:
            ARCH="i386" 
            ABI_FLAGS="-std=c++17 -fexceptions -frtti"
            ;;
		"x86_64")
			CCTYPE="x86_64-linux-android"
			ARCH_DIR="x86_64-linux-android"
			LIBPATH="x86_64-linux-android"
			ARCH="x86_64"
			TARGET="x86_64-linux-android"
			ABI_FLAGS="-march=x86-64 -std=c++17 -fexceptions -frtti"
			;;
    esac

	rm -rf src/*.o
    rm -rf android/build/obj/*
    rm -rf $NATIVE_APP_GLUE/*.o

    # --- Compila native app glue ---
	# Assicurati che $TARGET sia "i686-linux-android29" per x86
	$TOOLCHAIN/bin/${CCTYPE}29-clang \
		-target $TARGET \
		--sysroot=$TOOLCHAIN/sysroot \
		-c $NATIVE_APP_GLUE/android_native_app_glue.c \
		-o $NATIVE_APP_GLUE/native_app_glue.o \
		$INCLUDES \
		-I$TOOLCHAIN/sysroot/usr/include/$ARCH_DIR \
		$FLAGS -std=c99

	# Rigenera la libreria statica
	mkdir -p lib/$ABI
	rm -f lib/$ABI/libnative_app_glue.a
	$TOOLCHAIN/bin/llvm-ar rcs lib/$ABI/libnative_app_glue.a $NATIVE_APP_GLUE/native_app_glue.o

    # 3. COMPILAZIONE DEL TUO PROGETTO (.cpp)
    for file in src/*.cpp; do
        $TOOLCHAIN/bin/clang++ -target $TARGET \
            -c $file -o "$file".o \
            --sysroot=$TOOLCHAIN/sysroot \
            -I$TOOLCHAIN/sysroot/usr/include \
            -I$TOOLCHAIN/sysroot/usr/include/$ARCH_DIR \
            -stdlib=libc++ \
            $INCLUDES $FLAGS $ABI_FLAGS
    done

    # 4. LINKER (La parte "difficile")
    SYSROOT_LIB="$TOOLCHAIN/sysroot/usr/lib/$LIBPATH/29"
    CLANG_VER=$(ls $TOOLCHAIN/lib/clang/ | head -n 1)
    CLANG_LIB="$TOOLCHAIN/lib/clang/$CLANG_VER/lib/linux/$ARCH"

	rm -f android/build/lib/$ABI/libmain.so

    $TOOLCHAIN/bin/ld.lld src/*.cpp.o -o android/build/lib/$ABI/libmain.so -shared \
        --exclude-libs libatomic.a --build-id \
        -z noexecstack -z relro -z now \
        --warn-shared-textrel --fatal-warnings -u ANativeActivity_onCreate \
        -L$SYSROOT_LIB \
        -L$CLANG_LIB \
        -L$TOOLCHAIN/sysroot/usr/lib/$LIBPATH \
        -L. -Landroid/build/obj -Llib/$ABI \
        -lraylib -lnative_app_glue -llog -landroid -lEGL -lGLESv2 -lOpenSLES \
        -lc++_shared -latomic -lc -lm -ldl

    # 5. COPIA STL
    cp "$TOOLCHAIN/sysroot/usr/lib/$LIBPATH/libc++_shared.so" android/build/lib/$ABI/libc++_shared.so
done

echo "Compilazione completata per tutte le ABI: $ABIS"

# ______________________________________________________________________________
#
#  Build APK
# ______________________________________________________________________________
#
$BUILD_TOOLS/aapt package -f -m \
	-S android/build/res -J android/build/src -M android/build/AndroidManifest.xml \
	-I android/sdk/platforms/android-29/android.jar

# Compile NativeLoader.java
javac -verbose -source 1.8 -target 1.8 -d android/build/obj \
	-bootclasspath jre/lib/rt.jar \
	-classpath android/sdk/platforms/android-29/android.jar:android/build/obj \
	-sourcepath src android/build/src/com/raylib/game/R.java \
	android/build/src/com/raylib/game/NativeLoader.java

$BUILD_TOOLS/dx --verbose --dex --output=android/build/dex/classes.dex android/build/obj

# Add resources and assets to APK
$BUILD_TOOLS/aapt package -f \
	-M android/build/AndroidManifest.xml -S android/build/res -A assets \
	-I android/sdk/platforms/android-29/android.jar -F game.apk android/build/dex

# Add libraries to APK (libmain.so + libc++_shared.so per ogni ABI)
cd android/build
for ABI in $ABIS; do
	../../$BUILD_TOOLS/aapt add ../../game.apk lib/$ABI/libmain.so
	../../$BUILD_TOOLS/aapt add ../../game.apk lib/$ABI/libc++_shared.so
done
cd ../..

# Zipalign e firma
$BUILD_TOOLS/zipalign -f 4 game.apk game.final.apk
mv -f game.final.apk game.apk

apksigner sign --ks android/raylib.keystore --out my-app-release.apk --ks-pass pass:raylib game.apk
mv my-app-release.apk game.apk

# Installa sul dispositivo / emulatore
android/sdk/platform-tools/adb install -r game.apk