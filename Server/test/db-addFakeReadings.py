# from flask_sqlalchemy import SQLAlchemy
# from sqlalchemy import create_engine, MetaData, Table
# from sqlalchemy.orm import sessionmaker
# from datetime import datetime, timedelta

# from example_payload import generate_random_payload
# import src.app as app


# def generateFakePayloadTime(session, time="one day") -> dict:
#     RAPORT_TIME = 60  # one hour

#     if time == "one day":
#         time = datetime.utcnow() - timedelta(days=1)

#     while time < datetime.utcnow():
#         payload = generate_random_payload(time=time)
#         app.payload2db(payload, session)

#         time += timedelta(minutes=RAPORT_TIME)


# engine = create_engine('sqlite:///data.db')
# metadata = MetaData()

# # Reflect the tables
# metadata.reflect(bind=engine)

# # Create a session
# Session = sessionmaker(bind=engine)
# session = Session()

# generate_random_payload.counter = 0

# generateFakePayloadTime(session)
