for i in $(cat ips.txt);do scp $1  udesc@$i:~/ ; done;
