gnome-terminal -e '/bin/bash -c "./server TCP 4000 0 ok 70; exec bash"'

POSTGRESQL

sudo apt-get install libpqxx-dev
sudo apt-get install libpq-dev

g++ -o server server.cpp -lpqxx -lpq && source .env &&  ./server TCP 8080 1 0 1
