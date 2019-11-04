@echo off

del *.o
del *.bak
del *.exe
del *.bmp

cd vfont
call clean.bat
cd ..
