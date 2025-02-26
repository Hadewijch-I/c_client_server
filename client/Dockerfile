FROM alpine:latest

RUN apk add --no-cache gcc musl-dev vim nano

WORKDIR /client

COPY ./main.c ./users.csv .

RUN gcc -o client_app main.c

CMD ["sh", "-c", "while true; do sleep 3600; done"]
