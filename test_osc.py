import random
import time
from math import sin

from pythonosc import udp_client


if __name__ == "__main__":
    client = udp_client.SimpleUDPClient("127.0.0.1", 7000)

    for x in range(100, 1000000000):
        val = x
        print(f"Sending {val}")
        if x % 3 == 0:
            client.send_message("dsfsdlkfhsdjvgejsjgvdjddsfdsfdsf", val / 100)
        for name in ["/filter", "bob", "/lol"]:
            client.send_message(name, sin(val))
            time.sleep(1)
