zcc +msx -create-app -O3 -DAMALLOC -subtype=msxdos main.c libiot.c -bn VIBE.COM -lm
del VIBE.img
move /y VIBE.COM bin
cd bin
copy dosformsx.dsk VIBE.DSK
START /WAIT DISKMGR.exe -A -F -C VIBE.DSK VIBE.COM
cd ../
