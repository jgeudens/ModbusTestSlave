#!/bin/bash
set -ex

# Copy assets
cp misc/ModbusSim.desktop release/bin/linux/
cp misc/icon/icon-256x256.png release/bin/linux/ModbusSim.png
cd release/bin/linux/

# now, build AppImage using linuxdeploy and linuxdeploy-plugin-qt
# download linuxdeploy and its Qt plugin
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

# make them executable
chmod +x linuxdeploy*.AppImage

# Generate AppImage
export APPIMAGE_EXTRACT_AND_RUN=1 # Workaround because FUSE isn't available in Docker
./linuxdeploy-x86_64.AppImage --appdir AppDir -e modbussim -i ModbusSim.png -d ModbusSim.desktop --plugin qt --output appimage

ls

cp ModbusSim*.AppImage ../../..
