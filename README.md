# sony_cam_ros_noetic 
# Установка необходимых библиотек. Исходники находяться в ветке source

sudo apt-get update

sudo apt-get install automake autopoint gettext libtool libusb-dev libpopt-dev subversion libjpeg-dev libexif-dev

autoreconf --install --symlink

./configure --prefix=/usr/local

make

sudo make install

PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

LD_LIBRARY_PATH=/usr/local/lib

# Потом исходники можно удалить.

# Программы с управлением камерой компилировать g++ -lgphoto2 -lgphoto2_port
