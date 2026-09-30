#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  printf("Main Node: Received unicast message from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

PROCESS(main_node_process, "Main Node Process");
AUTOSTART_PROCESSES(&main_node_process);

PROCESS_THREAD(main_node_process, ev, data) {
  PROCESS_BEGIN();

  unicast_open(&uc, 146, &unicast_callbacks);

  printf("Main Node: ready to receive data\n");

  while(1) {
    PROCESS_WAIT_EVENT();
  }

  unicast_close(&uc);

  PROCESS_END();
}

