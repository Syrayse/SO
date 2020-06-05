#!/bin/bash
make

echo "Digite a quantiade de repetições: "
read ciclo

for (( i = 1 ; i <= ciclo ; i++ ))
do 
	./argus -e "ls -l"
	./argus -e "ls -l | cat | limit -n 3"
	./argus -e "ls -l | limit -n 3"
	./argus -e "grep -v ^# /etc/passwd | cut -f7 -d: | uniq | wc -l"
	./argus -e "ls /etc | wc -l"
done
