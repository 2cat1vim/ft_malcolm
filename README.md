# ft_malcolm

`ft_malcolm` is a lightweight ARP spoofing tool written in C. It listens on a network interface for a specific ARP request and replies with a spoofed ARP packet, allowing the source host to impersonate another host on the local network (classic ARP cache poisoning / Man-in-the-Middle setup).

A Docker-based lab (`middleman`, `victim`, `victim_target`) is included so the tool can be built and tested safely in an isolated network without touching real infrastructure.

> ⚠️ **Educational use only.** ARP spoofing can disrupt or intercept traffic on a real network. Only run this tool against systems and networks you own or are explicitly authorized to test.

---

## How it works

1. `ft_malcolm` opens a raw `AF_PACKET` socket on the detected network interface.
2. It waits for an ARP **request** matching the given target IP/MAC, asking about the given source IP.
3. Once seen, it crafts a spoofed ARP **reply**, claiming that `source-ip` lives at the attacker's own MAC address, and sends it directly to the target.
4. The target updates its ARP table, and traffic meant for `source-ip` is now routed to the attacker.

## Project structure

```
.
├── Makefile                 # Builds the ft_malcolm binary
├── include/
│   └── malcolm.h             # Shared types, structs (t_malcolm, ARP/Ethernet headers) and prototypes
├── src/
│   ├── main.c                 # Entry point, signal handling, main ARP wait/reply loop
│   ├── parse.c                 # CLI argument parsing/validation, interface resolution
│   ├── init.c                    # t_malcolm struct init/cleanup
│   └── packet.c                    # Raw socket creation, ARP request sniffing, reply crafting/sending
├── lib/                      # Small custom libc-style helper library (lib.a)
│   ├── lib.h
│   ├── exit.c                # Unified exit/error helpers (f_exit, ep_exit, sp_exit, ...)
│   ├── len.c                 # s_len (strlen replacement)
│   └── memory.c               # s_dup, zro_mem, cpy_mem, cmp_mem (mem* helpers)
└── docker/
    ├── docker-compose.yml    # 3-container lab network (192.168.1.0/24)
    ├── Makefile               # Convenience targets to spin the lab up/down
    ├── middleman/Dockerfile   # Container where ft_malcolm is built and run (the "attacker")
    ├── victim/Dockerfile      # Container with ping/arping/tcpdump for testing
    └── victim_target/Dockerfile
```

## Building

Requirements: `clang`, `make`.

```sh
make        # builds lib/lib.a then the ft_malcolm binary
make clean  # removes object files
make fclean # removes object files and the binary
make re     # fclean + all
```

## Usage

`ft_malcolm` must be run as **root** (it needs raw socket access), and takes exactly four arguments, in this order:

```sh
sudo ./ft_malcolm <source-ip> <source-mac> <target-ip> <target-mac>
```

| Argument      | Description                                                        |
|---------------|---------------------------------------------------------------------|
| `source-ip`   | The IP address you want to impersonate (the one the reply claims)  |
| `source-mac`  | The MAC address of the real host normally holding `source-ip`      |
| `target-ip`   | The IP address of the victim that will receive the spoofed reply    |
| `target-mac`  | The MAC address of the victim                                       |

Example:

```sh
sudo ./ft_malcolm 192.168.1.4 02:42:c0:a8:01:04 192.168.1.3 02:42:c0:a8:01:03
```

On startup, `ft_malcolm` auto-detects a usable network interface, waits for a matching ARP request from the target, then sends a spoofed reply and exits.

## Testing with Docker

A three-container lab is provided to safely reproduce an ARP spoofing scenario:

- **middleman** – builds and runs `ft_malcolm` (the attacker box)
- **victim** – has `ping`, `arping`, and `tcpdump` for generating and observing traffic
- **victim_target** – the second host on the network

```sh
cd docker
make        # builds and starts the 3 containers (network 192.168.1.0/24)
make clean  # tears the lab down
make fclean # tears down and prunes the Docker system
make restart
```

Once the containers are up, attach a shell to each (`docker exec -it <container> bash`), build `ft_malcolm` inside `middleman`, and trigger ARP requests from `victim`/`victim_target` (e.g. with `arping` or `ping`) to observe the spoofed reply with `tcpdump`.

## Disclaimer

This project was built for learning purposes (network programming, raw sockets, the ARP protocol). Do not use it against networks or devices without explicit permission.
