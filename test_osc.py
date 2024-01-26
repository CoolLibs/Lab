import random
import time
from math import sin

from pythonosc import udp_client


if __name__ == "__main__":
    client = udp_client.SimpleUDPClient("127.0.0.1", 7000)

    for x in range(0, 1000000000):
        val = x
        print(f"Sending {val}")
        # if x % 3 == 0:
        client.send_message("a", sin(val / 100) * 0.5 + 0.5)
        for name in ["/filter", "bob", "/lol"]:
            client.send_message(name, sin(val))
            # time.sleep(0.1)
