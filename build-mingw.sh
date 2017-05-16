#!/bin/sh

BITS=32

if [[ $# -ge 1 ]]; then
  case "$1" in
    --help|-h)
    echo "Usage: <script> [options]"
    echo -e "\nAvailable options:"
    echo -e "\t--help, -h\tThis usage explanation."
    echo -e "\t--32\t\tBuild 32-bit binary with MinGW-w64."
    echo -e "\t--64\t\tBuild 64-bit binary with MinGW-w64."
    echo -e "\t--clean, -c\tClean build artifacts."
    echo ""
    exit 0
    ;;
    --32)
    BITS=32
    ;;
    --64)
    BITS=64
    ;;
    --clean|-c)
    make clean
    rm -f *.dll
    exit 0
    ;;
    *)
    echo "Unknown option, use --help for usage instructions."
    ;;
  esac
  shift
fi

if [ "${BITS}" == "32" ]; then
    MINGW=i686-w64-mingw32
    LIBS="libgcc_s_sjlj-1 "
else
    MINGW=x86_64-w64-mingw32
    LIBS="libgcc_s_seh-1 "
fi

MINGW_PATH=/usr/${MINGW}
PATH=${MINGW_PATH}/bin:${MINGW_PATH}/sys-root/mingw/bin:${PATH}; echo ${PATH}
CC=${MINGW}-gcc

# Clean previous build artifacts
make clean
rm -f *.dll

make CC=${CC} EXEC=wine EXE=.exe $@

LIBS+="libbz2-1 libFLAC-8 libmodplug-1 libogg-0 libstdc++-6 libvorbis-0 libvorbisfile-3 libwinpthread-1 SDL2 SDL2_mixer SDL2_net zlib1"
for lib in ${LIBS}; do
  cp -f ${MINGW_PATH}/sys-root/mingw/bin/${lib}.dll .
done
