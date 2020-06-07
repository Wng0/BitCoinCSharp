#Copy from Bitcoin v0.1.5
ifneq "$(BUILD)" "debug"
ifneq "$(BUILD)" "release"
BUILD=debug
endif
endif
ifeq "$(BUILD)" "debug"
D=d
DEBUGFLAGS=-g -D__WXDEBUG__
endif
INCLUDEPATHS=-I"/boost" -I"/DB/build_unix" -I"/OpenSSL/include" -I"/wxWidgets/lib/vc_lib/mswd" -I"/wxWidgets/include"
LIBPATHS=-L"/DB/build_unix" -L"/OpenSSL/out" -L"/wxWidgets/lib/gcc_lib"
LIBS= \
 -l db_cxx \
 -l eay32 \
 -l wxmsw28$(D)_richtext -l wxmsw28$(D)_html -l wxmsw28$(D)_core -l wxbase28$(D) -l wxtiff$(D) -l wxjpeg$(D) -l wxpng$(D) -l wxzlib$(D) -l wxregex$(D) -l wxexpat$(D) \
 -l kernel32 -l user32 -l gdi32 -l comdlg32 -l winspool -l winmm -l shell32 -l comctl32 -l ole32 -l oleaut32 -l uuid -l rpcrt4 -l advapi32 -l ws2_32
WXDEFS=-DWIN32 -D__WXMSW__ -D_WINDOWS -DNOPCH
CFLAGS=-mthreads -O0 -w -Wno-invalid-offsetof -Wformat $(DEBUGFLAGS) $(WXDEFS) $(INCLUDEPATHS)
HEADERS=headers.h util.h main.h seriaalize.h uint256.h key.h bignum.h script.h db.h base58.h
all: bitcoin.exe
headers.h.gch: headers.h $(HEADERS) net.h irc.h market.h uibase.h ui.h
	g++ -c $(CFLAGS) -o $@ $<
obj/util.o: util.cpp			$(HEADERS)
	g++ -c $(CFLAGS) -o $@ $<
obj/script.o: script.cpp		$(HEADERS)
	g++ -c $(CFLAGS) -o $@ $<
obj/db.o: db.cpp				$(HEADERS) market.h
	g++ -c $(CFLAGS) -o $@ $<

































