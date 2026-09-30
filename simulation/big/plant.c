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
    case 6: return "ivy";
    case 7: return "jasmin";
    case 8: return "briar";
    case 9: return "iris";
    case 10: return "lavender";
    case 11: return "lily";
    case 12: return "balloon flower";
    case 13: return "barrel cactus";
    case 14: return "cherry blossom";
    case 15: return "dahlia";
    case 16: return "daisy";
    case 17: return "foxglove";
    case 18: return "geranium";
    case 19: return "magnolia";
    case 20: return "mosses";
    case 21: return "orchid";
    case 22: return "poppy";
    case 23: return "rose";
    case 24: return "sunflower";
    case 25: return "aloe vera";
    case 26: return "baby's breath";
    case 27: return "cactus";
    case 28: return "yarrow plant";
    case 29: return "calla lily";
    case 30: return "camellia";
    case 31: return "canna lily";
    case 32: return "banyan plant";
    case 33: return "basil plant";
    case 34: return "pine plant";
    case 35: return "tansy plant";
    case 36: return "tulip plant";
    case 37: return "chrysanthemum";
    case 38: return "celosia";
    case 39: return "carnation";
    case 40: return "daffodil";
    case 41: return "zinnia plant";
    case 42: return "garlic plant";
    case 43: return "carrot plant";
    case 44: return "dates plant";
    case 45: return "mango plant";
    case 46: return "sweet pea";
    case 47: return "forget me not";
    case 48: return "dandelion";
    case 49: return "freesia";
    case 50: return "hibiscus";
    case 51: return "snake plant";
    case 52: return "pansy";
    case 53: return "oak";
    case 54: return "money plant";
    default: return "Unknown";
  }
}

static struct unicast_conn uc;
static int received_ack = 0;

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
  char *received_data = (char *)packetbuf_dataptr();

  if (strcmp(received_data, "ACK") == 0) {
    printf("plant_ID %d: received ACK from server\n", linkaddr_node_addr.u8[0]);
    received_ack = 1;
  } else {
    printf("plant_ID %d: received wierd data: '%s'\n", linkaddr_node_addr.u8[0], received_data);
  }
}

static const struct unicast_callbacks unicast_callbacks = {recv_uc};

PROCESS_THREAD(plant_node_process, ev, data) {
  static struct etimer timer, ack_timer;

  PROCESS_EXITHANDLER(unicast_close(&uc));

  PROCESS_BEGIN();

  unicast_open(&uc, 146, &unicast_callbacks);
  etimer_set(&timer, CLOCK_SECOND * 120 + (random_rand() % CLOCK_SECOND * 10));

  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    received_ack = 0;
    send_data();
    etimer_set(&ack_timer, CLOCK_SECOND * 10);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&ack_timer) || received_ack);

    if (!received_ack) {
      printf("plant_ID %d: No ACK received resending data\n", linkaddr_node_addr.u8[0]);
      send_data();
    }

    etimer_set(&timer, CLOCK_SECOND * 120 + (random_rand() % CLOCK_SECOND * 10));
  }

  PROCESS_END();
}

