#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include <string.h>

static struct unicast_conn uc;

PROCESS(main_node_process, "Main Node Process");
AUTOSTART_PROCESSES(&main_node_process);

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  char *received_data = (char *)packetbuf_dataptr();

  printf("Main Node: received data from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], received_data);

  char ack_msg[] = "ACK";
  packetbuf_copyfrom(ack_msg, sizeof(ack_msg));
  unicast_send(&uc, from);

  printf("Main Node: Sent ACK to %d.%d\n", from->u8[0], from->u8[1]);
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};

PROCESS_THREAD(main_node_process, ev, data) {
  PROCESS_EXITHANDLER(unicast_close(&uc));

  PROCESS_BEGIN();

  unicast_open(&uc, 146, &unicast_callbacks);
  printf("main Node: ready to receive messages\n");

  PROCESS_END();
}
