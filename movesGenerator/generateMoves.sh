#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Uso: $0 rato-compilado mapa.json"
  exit 1
fi

CODE=$1
MAP=$2

# Create FIFO
FIFO=tmpfifo

if [ ! -p $FIFO ]; then
  mkfifo $FIFO
fi

# Run the command with timeout
timeout 3s ./$CODE < $FIFO | ./mg ../assets/maps/$MAP > $FIFO

# Erase FIFO
rm $FIFO
