TARGET = fomod-installer

CFLAGS = -Wall -Wpedantic $(shell pkg-config libxml-2.0 --cflags)
CXXFLAGS = $(CFLAGS)
LDFLAGS = 
LIBS = $(shell pkg-config libxml-2.0 --libs)

OBJS = parser.o

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJS) $(TARGET)
