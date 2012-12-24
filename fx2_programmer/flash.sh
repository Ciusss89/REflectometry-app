#!/bin/bash
# TIPALDI GIUSEPPE- 26-10-2012. r03

debug="1"
echo "Type 'scan' for looking gn3s v2 devices, and print busses"
echo "Type 'program' for load in ram new firmware."
echo "**Dependence fx2_programmer**"

read -p "Command : " buf 

if [  $buf = "scan" ];
then
	echo "----------------USB DEVICES-----------------------"
	fx2_programmer any any dump_busses
elif [ $buf = "program" ]; 
then
	BUS=`( fx2_programmer any any dump_busses | grep 0b39 | awk  '{ print $2 }' )`
	ID=`( fx2_programmer any any dump_busses | grep 0b39 | awk  '{ print $4 }' )`
		echo "----------------Start flashing procedure-----------------------"
		sleep 2
		 fx2_programmer $BUS $ID set 0xE600 1
		 fx2_programmer $BUS $ID program sige.ihx
		 fx2_programmer $BUS $ID set 0xE600 0
else 
	echo  ** Invalid paramitrer, [$buf] EXIT FAILURE 
fi

if [ $debug = "1" ];
then
	echo  "----------------KERNEL TAIL MESSAGE-----------------------"
	sleep 2; dmesg | tail
	echo  "--------------------------END-----------------------------"
fi
