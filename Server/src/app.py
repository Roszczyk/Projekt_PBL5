from flask_swagger import swagger
from flask_swagger_ui import get_swaggerui_blueprint
from flask import Flask, jsonify
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import DECIMAL

import paho.mqtt.client as mqtt

from datetime import datetime, timedelta
import json
from time import time


app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///data.db'
db = SQLAlchemy(app)


class Device_EUI(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    dev_eui = db.Column(db.String, unique=True)


class Data(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(db.Integer, db.ForeignKey('device_eui.id'))
    device = db.relationship('Device_EUI')

    timestamp = db.Column(db.DateTime, default=datetime.utcnow)
    temperature = db.Column(DECIMAL(precision=4, scale=1))
    humidity = db.Column(DECIMAL(precision=4, scale=1))
    gps_lat = db.Column(DECIMAL(precision=7, scale=4), nullable=True)
    gps_lon = db.Column(DECIMAL(precision=7, scale=4), nullable=True)


def payload2db(payload: str, session=db.session):
    payload = json.loads(payload)

    dev_eui = payload['dev_EUI']
    timestamp = datetime.fromisoformat(payload['received_at'])

    temperature = payload['decoded_payload']['temperature_0']
    humidity = payload['decoded_payload']['relative_humidity_0']

    if "gps_0" in payload["decoded_payload"]:
        gps_lat = payload['decoded_payload']['gps_0']['latitude']
        gps_lon = payload['decoded_payload']['gps_0']['longitude']
    else:
        gps_lat, gps_lon = None, None

    with app.app_context():
        # Check if the device already exists in the database
        device = Device_EUI.query.filter_by(dev_eui=dev_eui).first()

        # If the device doesn't exist, create a new one
        if device is None:
            device = Device_EUI(dev_eui=dev_eui)
            session.add(device)
            session.commit()

        # Create a new Data object and add it to the database
        data = Data(device_id=device.id, timestamp=timestamp,
                    temperature=temperature, humidity=humidity, gps_lat=gps_lat, gps_lon=gps_lon)
        session.add(data)
        session.commit()


def on_message(client, userdata, msg):
    payload = msg.payload.decode()
    print(time(), "payload received")
    payload2db(payload, db.session)


@app.route('/data/tempHum', methods=['GET'])
def get_tempHum():
    """
    Get temperature and humidity data for the last 24 hours.
    ---
    responses:
        200:
            description: A list of temperature and humidity data.
            schema:
                type: array
                items:
                    type: object
                    properties:
                        timestamp:
                            type: string
                            format: date-time
                        temperature:
                            type: number
                        humidity:
                            type: number
    """
    last_24h = datetime.utcnow() - timedelta(hours=24)
    data = Data.query.filter(Data.timestamp >= last_24h).all()
    if data:
        result = [{'timestamp': entry.timestamp, 'temperature': entry.temperature, 'humidity': entry.humidity}
                  for entry in data]
        print(time(), result[:5])
        return jsonify(result)
    else:
        return jsonify({'message': 'No data available.'}), 404


@app.route('/data/gps', methods=['GET'])
def get_gps():
    """
    Get current GPS location.
    ---
    responses:
        200:
            description: Current GPS location.
            schema:
                type: object
                properties:
                    timestamp:
                        type: string
                        format: date-time
                    gps_lat:
                        type: number
                    gps_lon:
                        type: number
    """
    data = Data.query.filter(Data.gps_lat.isnot(None), Data.gps_lon.isnot(
        None)).order_by(Data.timestamp.desc()).first()

    if data:
        result = {'timestamp': data.timestamp,
                  'gps_lat': data.gps_lat, 'gps_lon': data.gps_lon}
        print(time(), result)
        return jsonify(result)
    else:
        return jsonify({'message': 'No GPS location available.'}), 404


@app.route('/swagger.json')
def swagger_json():
    return jsonify(swagger(app))


if __name__ == '__main__':

    with app.app_context():
        db.create_all()

    client = mqtt.Client()
    client.on_message = on_message

    client.username_pw_set("rw", "readwrite")
    client.connect("test.mosquitto.org", 1884)
    topic_sub = "PAM-PBL5/RIOT-test-uplink"

    client.subscribe(topic_sub)
    client.loop_start()

    SWAGGER_URL = '/swagger'
    API_URL = '/swagger.json'

    swaggerui_blueprint = get_swaggerui_blueprint(
        SWAGGER_URL,
        API_URL,
        config={
            'app_name': "PAM Server"
        }
    )

    app.register_blueprint(swaggerui_blueprint, url_prefix=SWAGGER_URL)

    # NAPRAWIONE: debug=False musi być, bo inaczej callbacki mqtt są dwukrotnie.
    app.run(debug=False)
