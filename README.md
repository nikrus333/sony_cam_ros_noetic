# sony_cam_ros_noetic
sudo apt-get update

sudo apt-get install automake autopoint gettext libtool libusb-dev libpopt-dev subversion libjpeg-dev libexif-dev

autoreconf --install --symlink

./configure --prefix=/usr/local

make

sudo make install

PKG_CONFIG_PATH=/usr/local/lib/pkgconfig

LD_LIBRARY_PATH=/usr/local/lib

потом исходники можно удалить.

программы с управлением камерой компилировать g++ -lgphoto2 -lgphoto2_port
