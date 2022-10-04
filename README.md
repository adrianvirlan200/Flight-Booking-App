# Flight-booking
Make a flight search and booking app.
The application must have the following actors:
Operator: login activities and add/delete flights (the account already exists in the system with user and password)
User: account creation, login, flight search, booking activities

Implement exception handling methods for the following actions:
Operator: login failed (wrong user or password), adding wrong details to the flight (wrong date format, date in the past, country to have non-existent characters in country names)
User: login failed, authentication failed (incorrect email format, password too weak, repeated password not correct), non-existent flight, entering wrong details when booking a flight

Passwords for accounts must be encrypted using an RSA algorithm.
