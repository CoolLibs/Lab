import time
from math import sin
from pythonosc import udp_client

if __name__ == "__main__":
    client = udp_client.SimpleUDPClient("127.0.0.1", 7000)
    for x in range(0, 1000000000):
        print(f"Sending {x}")
        client.send_message("a", sin(x / 100) * 0.5 + 0.5)
        for name in ["/filter", "bob", "/lol"]:
            client.send_message(name, sin(x))
            time.sleep(0.1)
