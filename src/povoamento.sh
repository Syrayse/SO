#!/bin/bash

# todos os comandos.
commands=("ls -l" "cat /etc/passwd" "este_comand_nao_existe" "cat /etc/passwd | head -5 | sort" "sleep 5" "sleep 10" "sleep 15" "sleep 25" "sleep 30" "ls | sleep 1 | cat" "ls | sleep 5 | cat" "ls | sleep 10 | cat" "ls | sleep 15 | cat")

# A semente.
RANDOM=$$$(date +%s)

# Escolhe uma entrada.

COUNTER=0

while [ $COUNTER -lt $1 ]; do
	DOMAIN_NAME_TO_CHECK=${commands[$RANDOM % ${#commands[@]}]}
	./argus -e "${DOMAIN_NAME_TO_CHECK}"
	let COUNTER+=1
done