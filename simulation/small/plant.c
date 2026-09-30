#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  printf("Unicast message received from %d.%d: '%s'\n",
         from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};
static struct unicast_conn uc;

PROCESS(plant_node_process, "Plant Node Process");
AUTOSTART_PROCESSES(&plant_node_process);

PROCESS_THREAD(plant_node_process, ev, data) {
  static struct etimer timer;
  static char sensor_data[30];
  static linkaddr_t addr;

  PROCESS_BEGIN();

  unicast_open(&uc, 146, &unicast_callbacks);

  addr.u8[0] = 1;
  addr.u8[1] = 0;

  etimer_set(&timer, CLOCK_SECOND * 5);

  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    sprintf(sensor_data, "Temp:25C, Soil:40%%");
    packetbuf_copyfrom(sensor_data, sizeof(sensor_data));
    unicast_send(&uc, &addr);
    printf("Plant Node: Sent data - %s\n", sensor_data);
    etimer_reset(&timer);
  }

  unicast_close(&uc);

  PROCESS_END();
}

