import random
import json
from datetime import datetime


def static_vars(**kwargs):
    def decorate(func):
        for k in kwargs:
            setattr(func, k, kwargs[k])
        return func
    return decorate


payload_str = """
{
    "f_port": 1,
    "frm_payload": "AGcBEwBoRgCIBcOAGcBDQBoRQ==",
    "decoded_payload": {
    },
    "rx_metadata": [
        {
            "gateway_ids": {
                "gateway_id": "test"
            },
            "rssi": 42,
            "channel_rssi": 42,
            "snr": 4.2
        }
    ],
    "settings": {
        "data_rate": {
            "lora": {
                "bandwidth": 125000,
                "spreading_factor": 7
            }
        },
        "frequency": "868000000"
    }
}
"""


@static_vars(counter=0)
def generate_random_payload(time="now", dev_eui="70B3D57ED0063437"):

    def randomnes(s):
        return random.uniform(-s, s)

    data = json.loads(payload_str)
    generate_random_payload.counter += 1

    data["decoded_payload"]["temperature_0"] = 34.5 + randomnes(0.5)
    data["decoded_payload"]["relative_humidity_0"] = 25.4 + randomnes(0.5)
    data["dev_EUI"] = dev_eui

    if time == "now":
        time = datetime.utcnow()

    formatted_time = time.strftime("%Y-%m-%dT%H:%M:%S.%f")[:-3]
    data["received_at"] = formatted_time
    print(data["received_at"])

    if generate_random_payload.counter % 2 == 0:
        data["decoded_payload"]["gps_0"] = {}
        data["decoded_payload"]["gps_0"]["latitude"] = 37.7749 + randomnes(0.0001)
        data["decoded_payload"]["gps_0"]["longitude"] = - 122.4193 + randomnes(0.0001)
        data["decoded_payload"]["gps_0"]["altitude"] = 0

    if generate_random_payload.counter % 3 == 0:
        data["decoded_payload"]["digital_in_0"] = generate_random_payload.counter % 2

    data["decoded_payload"]["digital_in_1"] = generate_random_payload.counter % 5 < 2
    data["decoded_payload"]["digital_in_2"] = (1 + generate_random_payload.counter) % 5 < 3

    return json.dumps(data)


if __name__ == '__main__':
    print(generate_random_payload())
