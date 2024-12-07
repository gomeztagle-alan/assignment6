List of To-Dos:
- ~~Reverse engineer client.py~~
- Make sure xml tags don't receive \r, only POST methods
- Reverse engineer how server.py handles xml protocol messages
- Multiply two numbers and send to client

## Reverse Engineer Client.py
- copy xml response from client.py using echoserver (saved in xml_response.txt)
- ~~send identical response from client.c to echoserver~~
- send xml_response with proper \r \n
> POST messages receive \r, while xml tags don't

[ ] fix double connection for client.c

>Connected to (localhost, 54446)
>server received 31 bytes
>Connectedto (localhost, 34664)

## Reverse Engineer Server.py
