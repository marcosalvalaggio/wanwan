@echo off
rem Variables
set CC=clang
set CFLAGS=-Wall -Wextra -std=c11
set SRC_DIR=src
set OBJ_DIR=obj
set TARGET=wanwan.exe

rem Create the object directory if it doesn't exist
if not exist %OBJ_DIR% mkdir %OBJ_DIR%

rem Compile main.c into main.o
%CC% %CFLAGS% -c -o %OBJ_DIR%\main.o %SRC_DIR%\main.c

rem Link main.o into main.exe
%CC% -o %TARGET% %OBJ_DIR%\main.o

rem Print success message
echo Compilation and linking complete. Executable: %TARGET%
