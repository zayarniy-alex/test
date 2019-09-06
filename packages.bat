set VCPKG_DEFAULT_TRIPLET=x64-windows-static
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
