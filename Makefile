.PHONY:clean
CC=g++
CFLAGS=-Wall -g -std=c++0x
BIN=libMaple.a
OBJS=MutexLock.o Condition.o \
	 Thread.o ThreadPool.o \
	 Timer.o TimerThread.o \
	 InetAddress.o Socket.o \
	 SocketIO.o TcpConnection.o \
	 EpollPoller.o TcpServer.o
$(BIN):$(OBJS)
	ar -crv $@ -o $^
	chmod +x $@
	mkdir Maple
	cp *h Maple/
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf *.o core Maple
