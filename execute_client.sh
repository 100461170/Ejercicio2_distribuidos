#!/bin/bash
export IP_TUPLAS=localhost PORT_TUPLAS=4200;
if [ "$1" = "cliente" ]; then
    ./cliente
elif [ "$1" = "secuencial" ]; then
    ./prueba_sec
else
    echo "El primer argumento tiene que ser 'cliente' o 'secuencial'"
fi