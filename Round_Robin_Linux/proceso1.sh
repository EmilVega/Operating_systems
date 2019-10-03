#!/bin/bash

CONTADOR=0
while [  $CONTADOR -lt 5 ]; do
	echo El contador 1 es $CONTADOR
	let CONTADOR=CONTADOR+1
	sleep 4
done
