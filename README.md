# TCP client and Server in C language
## How to run
### Very simple one

1. Following command will run a MySQL database, Server and Client docker container and add them in a bridge network
    ```bash
    $ docker compose up
    ```

2. Exec into the client container and run the binary after adding the data in the csv file.
    ```bash
    $ docker exec -it c_client sh
    $ ./client_app
    ```

3. Check the MySQL database (Database: `mydatabase`, Table: `users`)


### Old school style
Visually described in the video: [Link](https://www.loom.com/share/52140ea05254489ea9f0278721ec4c08?sid=bc0dd4a0-6565-40e8-a704-2fa0f779205a)
