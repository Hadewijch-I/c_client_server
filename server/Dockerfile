FROM alpine:latest

RUN apk add --no-cache gcc g++ make cmake mariadb-connector-c-dev mysql-client

WORKDIR /app

COPY ./main.c .

RUN gcc -o my_server main.c -lmysqlclient

EXPOSE 8080

CMD ["./my_server"]
