# Build
```
git clone https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git
```
```
sudo apt install flex
```
```
sudo apt install bison
```
```
sudo apt install libelf-dev
```
```
make defconfig
```
```
make -j6 bzImage
```
```
make -j6 modules
```
```
make modules_install
```
```
make install
```
# Cross compile
```
make disclean // очистил после обычной сборки ядра
```
```
sudo apt install gcc-10-arm-linux-gnueabihf
```
```
ARCH=arm make defconfig
```
```
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j6 zImage
```
```
ARCH=arm make dtbs
```
```
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -nographic
```
# RootFS
```
arm-linux-gnueabihf-gcc -static init.c -o init
```
```
echo init | cpio -o -H newc | gzip > initramfs.cpio.gz 
```
```
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -initrd initramfs.cpio.gz -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0" -nographic
```
# RootFS busybox
```
ARCH=arm make defconfig
```
```
ARCH=arm make menuconfig
```
```
make -j6
```
```
make install
```
```
find . | cpio -o -H newc | gzip > initramfs.cpio.gz
```
```
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -initrd initramfs.cpio.gz -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0 rdinit=/bin/ash" -nographic
```
# OpenSSH
## zLib
```
CC=arm-linux-gnueabihf-gcc ./configure --static --prefix=$PWD/_install
```
```
make -j6
```
```
make install
```
## OpenSSL
```
./Configure linux-generic32 no-shared no-dso --cross-compile-prefix=arm-linux-gnueabihf- --prefix=$PWD/_install --openssldir=$PWD/_install/ssl -fPIC
```
```
make -j6
```
```
make install
```
## OpenSSH
```
autoreconf
```
```
./configure --disable-strip --disable-etc-default-login --host=arm-linux-gnueabihf --prefix=$PWD/_install --with-zlib=$PWD/../zlib/_install --with-ssl-dir=$PWD/../openssl/_install
```
```
make -j6
```
```
make install
```
Все что насобирали, закидываем в папку с busybox
```
find . | cpio -o -H newc | gzip > initramfs.cpio.gz
```
```
QEMU_AUDIO_DRV=none qemu-system-arm -M vexpress-a9 -kernel zImage -initrd initramfs.cpio.gz -dtb vexpress-v2p-ca9.dtb -append "console=ttyAMA0 rdinit=/bin/ash" -m 512M -nographic 
```
