all:thread_tcp
thread_tcp:thread_tcp.cpp
	g++ $^ -o $@ -lpthread
process_tcp:process_tcp.cpp
	g++ $^ -o $@
tcp_cli:tcp_cli.cpp
	g++ $^ -o $@
tcp_srv:tcp_srv.cpp
	g++ $^ -o $@
udp_cli:udp_cli.cpp
	g++ $^ -o $@
udp_srv:udp_srv.c
	gcc $^ -o $@
