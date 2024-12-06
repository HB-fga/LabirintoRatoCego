#!/bin/bash

if [ "$#" -lt 2 ]; then
  echo "Uso: $0 \"arquivo-executavel\" mapa.json [sigla-idioma]" 
  exit 1
fi

CODE=$1
MAP=$2
LANG=${3:-pt}

# Create FIFO
FIFO=tmpfifo

if [ ! -p $FIFO ]; then
  mkfifo $FIFO
fi

# Run the command with timeout
timeout 3s $CODE < $FIFO | ./mg ../assets/maps/$MAP > $FIFO -l $LANG

# Erase FIFO
rm $FIFO
