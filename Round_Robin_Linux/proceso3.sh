#!/bin/bash

CONTADOR=0
while [  $CONTADOR -lt 5 ]; do
	echo El contador 3 es $CONTADOR
	let CONTADOR=CONTADOR+1
	sleep 2
done
