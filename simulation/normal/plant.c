#include "contiki.h"
#include "net/rime/rime.h"
#include "random.h"
#include <stdio.h>

int get_temperature() { return 20 + random_rand() % 10; }
int get_humidity() { return 50 + random_rand() % 10; }
int get_soil_moisture() { return 30 + random_rand() % 10; }
int get_light_intensity() { return 70 + random_rand() % 10; }

const char *get_plant_name(uint8_t node_id) {
  switch (node_id) {
    case 2: return "strawberry";
    case 3: return "tomato";
    case 4: return "bamboo";
    case 5: return "fern";
    default: return "Unknown";
  }
}

static struct unicast_conn uc;

PROCESS(plant_node_process, "Plant Node Process");
AUTOSTART_PROCESSES(&plant_node_process);

static void send_data() {
  char buffer[64];
  int temp = get_temperature();
  int hum = get_humidity();
  int soil = get_soil_moisture();
  int light = get_light_intensity();

  uint8_t node_id = linkaddr_node_addr.u8[0];

  const char *plant_name = get_plant_name(node_id);

  snprintf(buffer, sizeof(buffer), "plant_ID:%d (%s) temp:%d hum:%d soil:%d light:%d",
           node_id, plant_name, temp, hum, soil, light);

  linkaddr_t addr;
  addr.u8[0] = 1;
  addr.u8[1] = 0;

  packetbuf_copyfrom(buffer, sizeof(buffer));
  unicast_send(&uc, &addr);
  printf("plant_ID %d: Sent data - %s\n", node_id, buffer);
}

static void recv_uc(struct unicast_conn *c, const linkaddr_t *from) {
  printf("Plant Node %d: Received reply from %d.%d: '%s'\n",
         linkaddr_node_addr.u8[0], from->u8[0], from->u8[1], (char *)packetbuf_dataptr());
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};

PROCESS_THREAD(plant_node_process, ev, data) {
  static struct etimer timer;

  PROCESS_EXITHANDLER(unicast_close(&uc));

  PROCESS_BEGIN();

  unicast_open(&uc, 146, &unicast_callbacks);
  etimer_set(&timer, CLOCK_SECOND * 10);

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    send_data();
    etimer_reset(&timer);
  }

  PROCESS_END();
}

