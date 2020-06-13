#!/bin/bash

# todos os comandos.
commands=("ls -l" "cat /etc/passwd" "este_comand_nao_existe" "cat /etc/passwd | head -5 | sort" "grep -v ^# /etc/passwd | cut -f7 -d: | uniq | wc -l" "sleep 15" "sleep 25" "sleep 30" "ls /etc | wc -l" "ls | sleep 5 | cat" "ls | sleep 10 | cat" "ls | sleep 15 | cat")

# A semente.
RANDOM=$$$(date +%s)

# Escolhe uma entrada.

COUNTER=0

while [ $COUNTER -lt $1 ]; do
	DOMAIN_NAME_TO_CHECK=${commands[$RANDOM % ${#commands[@]}]}
	./argus -e "${DOMAIN_NAME_TO_CHECK}"
	let COUNTER+=1
done
