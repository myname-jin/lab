# Makefile (project/Makefile)
CC = gcc
AR = ar
CFLAGS = -I./libsrc -pg
LIBDIR = ./libsrc
LIBSRC = $(LIBDIR)/arithmetic.c
LIBOBJ = $(LIBDIR)/arithmetic.o
LIBSTATIC = $(LIBDIR)/libarithmetic.a
LIBSHARED = $(LIBDIR)/libarithmetic.so
MAIN = main.c
TARGET_STATIC = main_static
TARGET_SHARED = main_shared

all: static shared

# 1. 목적 파일 만들기
$(LIBOBJ): $(LIBSRC)
	$(CC) -c $(LIBSRC) -o $(LIBOBJ)

# 2. 정적 라이브러리 만들기
static: $(LIBOBJ) $(MAIN)
	$(AR) rcs $(LIBSTATIC) $(LIBOBJ)
	$(CC) $(MAIN) -L$(LIBDIR) -larithmetic $(CFLAGS) -o $(TARGET_STATIC)

# 3. 공유 라이브러리 만들기
shared: $(LIBSRC) $(MAIN)
	$(CC) -fPIC -c $(LIBSRC) -o $(LIBOBJ)
	$(CC) -shared -o $(LIBSHARED) $(LIBOBJ)
	$(CC) $(MAIN) -L$(LIBDIR) -larithmetic $(CFLAGS) -o $(TARGET_SHARED)

# 4. 실행 파일 삭제
clean:
	rm -f $(LIBDIR)/*.o $(LIBDIR)/*.a $(LIBDIR)/*.so main_static main_shared
