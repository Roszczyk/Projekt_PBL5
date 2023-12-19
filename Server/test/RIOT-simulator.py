import paho.mqtt.client as mqtt
import time

from example_payload import generate_random_payload


def static_vars(**kwargs):
    def decorate(func):
        for k in kwargs:
            setattr(func, k, kwargs[k])
        return func
    return decorate


def on_connect(client, userdata, flags, rc):
    print("Connected with result code: " + str(rc))


@static_vars(counter=0)
def on_publish(client, userdata, mid):
    print(f"Message published {on_publish.counter}")
    on_publish.counter += 1


if __name__ == '__main__':
    client = mqtt.Client()

    client.on_connect = on_connect
    client.on_publish = on_publish

    client.username_pw_set("rw", "readwrite")
    client.connect("test.mosquitto.org", 1884)  # 91.121.93.94

    # client.loop_start()

    topic = "PAM-PBL5/RIOT-test-uplink"

    # Keep the script running
    try:
        while True:

            message = generate_random_payload()
            client.publish(topic, message)
            time.sleep(10)

    except KeyboardInterrupt:
        client.disconnect()
        client.loop_stop()
