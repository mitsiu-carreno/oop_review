#!/bin/bash

gnome-terminal --window --tab -t 't1' -e '/bin/bash -c "echo ./server TCP 8000 0 end 70; exec /bin/bash"' \
                        --tab -t 't2' -e '/bin/bash -c "echo ./server UDP 8300 28 0 70; exec /bin/bash"' \
                        --tab -t 't3' -e '/bin/bash -c "echo ./server UDP 8080 14 0 70; exec /bin/bash"' \
                        --tab -t 't4' -e '/bin/bash -c "echo ./server UDP 8900 0 finish 70; exec /bin/bash"' \
                        --tab -t 't5' -e '/bin/bash -c "echo ./server TCP 9100 30 0 70; exec /bin/bash"' \
                        --tab -t 't6' -e '/bin/bash -c "echo ./server UDP 9091 0 fin 70; exec /bin/bash"' \
                        --tab -t 't7' -e '/bin/bash -c "echo ./server UDP 9500 33 0 70; exec /bin/bash"' \
                        --tab -t 't8' -e '/bin/bash -c "echo ./server TCP 9900 12 0 70; exec /bin/bash"' \
                        --tab -t 't9' -e '/bin/bash -c "echo ./server TCP 10000 18 070; exec /bin/bash"' \
                        --tab -t 't10' -e '/bin/bash -c "echo ./server UDP 10101 0 thats_a_copy 70; exec /bin/bash"' \
                        --tab -t 't11' -e '/bin/bash -c "echo ./server TCP 10110 25 0 70; exec /bin/bash"' \
                        --tab -t 't12' -e '/bin/bash -c "echo ./server UDP 10001 0 ack 70; exec /bin/bash"' \
                        --tab -t 't13' -e '/bin/bash -c "echo ./server TCP 11010 19 0 70; exec /bin/bash"' \
                        --tab -t 't14' -e '/bin/bash -c "echo ./server TCP 11011 0 over 70; exec /bin/bash"' \
                        --tab -t 't15' -e '/bin/bash -c "echo ./server TCP 11100 35 0 70; exec /bin/bash"' \
                        --tab -t 't16' -e '/bin/bash -c "echo ./server UDP 11111 0 cambio_fuera 70; exec /bin/bash"'
