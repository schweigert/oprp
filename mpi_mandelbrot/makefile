build:
	mpic++ marlonbrot.c -fopenmp

run:
	mpirun --mca btl_tcp_if_include enp2s0 -n 2 -machinefile ips.txt a.out 20 23 20
