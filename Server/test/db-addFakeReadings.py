# ta struktura try exepot dla iomportu jest po to, aby autoformater nie przestawiał importów nad sys.path.insert - tak musi być

try:
    import sys
    import os
    sys.path.insert(0, os.path.abspath(
        os.path.join(os.path.dirname(__file__), '..')))
finally:
    from datetime import datetime, timedelta
    from example_payload import generate_random_payload
    from src import app


def generateFakePayloadTime(session, time="one day") -> dict:
    RAPORT_TIME = 60  # one hour

    if time == "one day":
        time = datetime.utcnow() - timedelta(days=1)

    while time < datetime.utcnow():
        payload = generate_random_payload(time=time)
        app.payload2db(payload, session)

        time += timedelta(minutes=RAPORT_TIME)


if __name__ == '__main__':

    with app.app.app_context():
        app.db.create_all()
    generateFakePayloadTime(app.db.session)
