CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++11

OBJS =		src/client.o src/helper.o

LIBS =

TARGET =	client

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
