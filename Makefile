MIX = mix
CFLAGS = -O3 -Wall

ERLANG_PATH = $(shell erl -eval 'io:format("~s", [lists:concat([code:root_dir(), "/erts-", erlang:system_info(version), "/include"])])' -s init stop -noshell)
CFLAGS += -I$(ERLANG_PATH)

ifeq ($(wildcard deps/pigpio),)
	PIGPIO_PATH = ../pigpio
else
	PIGPIO_PATH = deps/pigpio
endif

CFLAGS += -I$(PIGPIO_PATH) -fPIC
LDFLAGS = -lpthread -lrt

.PHONY: all ex_pigpio clean

all: ex_pigpio

ex_pigpio:
	$(MIX) compile

priv/ex_pigpio.so: src/ex_pigpio.c
	$(MAKE) -C $(PIGPIO_PATH) libpigpio.a
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ src/ex_pigpio.c $(PIGPIO_PATH)/libpigpio.a

clean:
	$(MIX) clean
	$(MAKE) -C $(PIGPIO_PATH) clean
	$(RM) priv/ex_pigpio.so
