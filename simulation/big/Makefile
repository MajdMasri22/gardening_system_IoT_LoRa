CONTIKI = ../../
TARGET ?= sky
MODULES += core/net/rime core/net
CONTIKI_WITH_IPV6 = 0
CFLAGS += -DNETSTACK_CONF_WITH_RIME=1
CFLAGS += -Os
CFLAGS += -DMEMORY_OPTIMIZED

all: plant server

plant: plant.c
	$(MAKE) -f $(CONTIKI)/Makefile TARGET=$(TARGET) $@

server: server.c
	$(MAKE) -f $(CONTIKI)/Makefile TARGET=$(TARGET) $@

include $(CONTIKI)/Makefile.include
