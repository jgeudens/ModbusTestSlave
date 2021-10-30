#!/bin/bash
set -ex

# now, build AppImage using linuxdeploy and linuxdeploy-plugin-qt
# download linuxdeploy and its Qt plugin
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

# make them executable
chmod +x linuxdeploy*.AppImage

# Copy assets
cp misc/ModbusSim.desktop release/bin/linux/
cp misc/ModbusSim.png release/bin/linux/

# Generate AppImage
./linuxdeploy-x86_64.AppImage --appdir AppDir -e modbussim -i ModbusSim.png -d ModbusSim.desktop --plugin qt --output appimage
