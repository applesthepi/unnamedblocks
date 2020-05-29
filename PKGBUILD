# Maintainer: The-Emperor10
pkgname=unnamedblocks
pkgver=0.1
pkgrel=1
pkgdesc="A modern, moddable, block based visual language "
arch=('x86_64')
url='https://kikoho.ddns.net/meme.html'
provides=('unnamedblocks')
license=('GPL3')
depends=('intel-tbb' 'csfml' 'sfml' 'glew' 'tcc' 'libx11')
makedepends=('gendesk')
optdepends=('')

build() {
	cmake -DCMAKE_INSTALL_PREFIX="$pkgdir/usr" -DCMAKE_BUILD_TYPE=Debug -DIS_BETA=1 -DRUNTIME_FOLDER_PATH='${XDG_DATA_HOME}/unnamedblocks' -DVERSION_MOD_VIN="$pkgver" -DVERSION_UNNAMED_BLOCKS="$pkgver" ..
	make
	# Generate desktop file automatically
	gendesk --icon=/usr/share/pixmaps/unnamedblocks.png -f
}
package() {
	mkdir -p "$pkgdir/usr/bin" "$pkgdir/usr/share/pixmaps" "$pkgdir/usr/share/applications"
	# Install desktop file
	install -Dm644 "$pkgname.desktop" "$pkgdir/usr/share/applications/$pkgname.desktop"
	# Binary Symbolic link
	ln -s /usr/share/unnamedblocks/unnamedblocks "$pkgdir/usr/bin/"
	# Icon symbolic link
	ln -s /usr/share/unnamedblocks/res/logo/logo_1024.png "$pkgdir/usr/share/pixmaps/$pkgname.png"
	make install
}

