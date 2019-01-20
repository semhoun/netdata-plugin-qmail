PREFIX ?= /usr
PLUGIN_DIR = ${DESTDIR}${PREFIX}/libexec/netdata/plugins.d

VERSION = 0.0

CFLAGS ?= -O2 -pipe
CFLAGS += -Wall -pedantic
CFLAGS += -std=c11
CFLAGS += -Werror=implicit-function-declaration

CPPFLAGS += -D_GNU_SOURCE

BIN = qmail.plugin smtpd.plugin

.PHONY: all
all: $(BIN)

## Dependencies
smtpd.plugin: err.o flush.o smtpd.plugin.o netdata.o
qmail.plugin: flush.o

err.o: err.c err.h
flush.o: flush.c flush.h
smtpd.plugin.o: err.h flush.h netdata.h
netdata.o: netdata.c netdata.h

.PHONY: install
install: all
	@echo installing executables to $(PLUGIN_DIR)
	install -d $(PLUGIN_DIR)
	install $(BIN) $(PLUGIN_DIR)

.PHONY: clean
clean:
	$(RM) *.o $(BIN)
