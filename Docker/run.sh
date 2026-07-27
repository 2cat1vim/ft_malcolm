#!/bin/bash

gnome-terminal -- bash -c "docker build --tag 'bot1' . && docker run -it --network host --cap-add=NET_RAW --cap-add=NET_ADMIN bot1 bash; exec bash"
gnome-terminal -- bash -c "docker build --tag 'bot2' . && docker run -it --network host --cap-add=NET_RAW --cap-add=NET_ADMIN bot2 bash; exec bash"
