# Gardening System: IoT with LoRa and Microcontroller Networks

## Overview

A group graduation project that integrates IoT, ESP32 microcontrollers, sensors, LoRa communication, and cloud monitoring to support remote plant monitoring and automated plant care.

The physical system collects environmental data such as temperature, humidity, soil moisture, and light intensity. The data is transmitted through the network and monitored using Arduino IoT Cloud, with fuzzy logic used for plant-care decisions.

## System Architecture

```text
Plant Nodes
(ESP32 + Sensors)
       │
    ESP-NOW
       │
       ▼
Garden LoRa ESP32
       │
      LoRa
       │
       ▼
Server LoRa ESP32
       │
       ▼
Arduino IoT Cloud
```

The physical system uses ESP-NOW for short-range communication between plant nodes and LoRa for communication with the central gateway.

## Cooja / Contiki Simulation

My primary individual contribution to the project was developing and testing the wireless network simulation using **Cooja and Contiki**.

The simulation was developed in multiple configurations:

* **Small:** One plant node and one server using Rime unicast communication, with data transmitted every 5 seconds.
* **Normal:** Four plant nodes and one server with simulated temperature, humidity, soil moisture, and light-intensity data, transmitted every 10 seconds.
* **Big:** Fifty plant nodes and one server using Rime communication, with simulated sensor data, acknowledgements (ACKs), and retransmission handling.

The simulated sensor values are generated in software and do not represent measurements from physical sensors.

## My Contribution

* Developed the Contiki/C code for plant nodes and server nodes.
* Implemented Rime unicast communication between simulated nodes.
* Generated simulated environmental sensor data.
* Scaled the simulation from a small network to 50 plant nodes.
* Implemented ACK and retransmission handling for the large-scale simulation.
* Created Makefiles for building the simulation configurations.
* Used Cooja to simulate and evaluate the wireless sensor network.

The physical hardware and overall IoT system were developed as a team project.

## Technologies

* C
* Contiki
* Cooja
* Rime
* ESP32
* LoRa
* ESP-NOW
* Arduino IoT Cloud
* Arduino IDE
* Fuzzy Logic
* Environmental Sensors

## Project Structure

```text
gardening-system-iot-lora/
├── README.md
└── simulation/
    ├── small/
    │   ├── plant.c
    │   ├── server.c
    │   └── Makefile
    ├── normal/
    │   ├── plant.c
    │   ├── server.c
    │   └── Makefile
    └── big/
        ├── plant.c
        ├── server.c
        └── Makefile
```

## Future Improvements

* Implement ESP32 sleep modes for improved power efficiency.
* Explore machine learning for plant classification and care recommendations.
* Develop a dedicated mobile application.
* Add GPS and solar-power capabilities.
