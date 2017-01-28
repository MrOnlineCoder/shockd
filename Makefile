CC=gcc
CFLAGS=-std=c99 -o shockd -w

WIN32_LIBS=-lws2_32
LINUX_LIBS=-pthread

SRC=main.c request.c response.c error.c config_file.c cache.c log.c

all:
	@echo "Use make win32 to build shockd for Windows"
	@echo "Or make linux to build for Linux"

win32:
	@echo "Building shockd for Windows NT..."
	$(CC) $(CFLAGS) $(SRC) $(WIN32_LIBS) 
	@echo "Done."

linux:
	@echo "Building shockd for Linux..."
	$(CC) $(CFLAGS) $(SRC) $(LINUX_LIBS) 
	@echo "Done."

