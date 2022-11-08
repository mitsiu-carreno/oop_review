#!/bin/bash

gnome-terminal --window --tab -t 't1' -e '/bin/bash -c "echo ./server TCP 8000 0 end 70; exec /bin/bash"' \
                        --tab -t 't2' -e '/bin/bash -c "echo ./server UDP 8300 28 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 8080 14 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 8900 0 finish 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 9100 30 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 9091 0 fin 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 9500 33 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 9900 12 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 10000 18 070; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 10101 0 thats_a_copy 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 10110 25 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 10001 0 ack 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 11010 19 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 11011 0 over 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server TCP 11100 35 0 70; exec /bin/bash"'
#gnome-terminal -e '/bin/bash -c "echo ./server UDP 11111 0 cambio_fuera 70; exec /bin/bash"'
