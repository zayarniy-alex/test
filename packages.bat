set VCPKG_DEFAULT_TRIPLET=x64-windows
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
git pull
call bootstrap-vcpkg.bat
vcpkg install zlib
vcpkg install openssl
vcpkg install pthreads
vcpkg install boost-signals2
vcpkg install boost-asio
vcpkg install boost-logic
vcpkg install boost-crc
vcpkg install boost-assign
vcpkg install sqlite3
vcpkg install curl
vcpkg install lua
vcpkg integrate project
 