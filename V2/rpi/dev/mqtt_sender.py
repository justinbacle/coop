import os
import sys
import itertools

sys.path.append(os.getcwd())
import V2.config as config  # noqa E402
import paho.mqtt.publish as publish  # noqa E402


def mqttPublish(path: str, data):
    publish.single(
        path,
        data,
        hostname=config.MQTT_SERVER_IP
    )


spinner = itertools.cycle(['-', '/', '|', '\\'])


if __name__ == "__main__":
    import string
    import random
    import time
    while True:
        for tempSensor in list(string.ascii_lowercase):
            _temp = random.randint(-10, 80)
            mqttPublish(
                "temp/" + tempSensor,
                _temp
            )
        sys.stdout.write(next(spinner))   # write the next character
        sys.stdout.flush()                # flush stdout buffer (actual character display)
        sys.stdout.write('\b')            # erase the last written char
        time.sleep(0.1)
