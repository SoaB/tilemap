TARGET = main
HEADFILE = 

WORKDIR = %cd%

CC = gcc
LD = gcc

WINDRES = windres.exe

INC = -I$(MINGW_DEV_LIB)\include
CFLAGS = -O2 -Wall -std=c99 -m64 -static

LIB = -L$(MINGW_DEV_LIB)\lib
LDFLAGS = -O2 -s -m64 -lraylibdll -lopengl32 -lgdi32 -lwinmm ###-mwindows

${TARGET}: ${TARGET}.o
	${CC} ${TARGET}.o ${LIB} ${LDFLAGS} -o ${TARGET}

${TARGET}.o: ${TARGET}.c ${HEADFILE}
		${CC} -c ${TARGET}.c ${CFLAGS} ${INC}

clean: 
	cmd /c del /f *.o
	cmd /c del /f *.exe
