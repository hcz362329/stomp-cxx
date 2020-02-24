DEFAULT: all

CXX = c++
CXXFLAGS += -std=c++17

SOURCES = socket.cpp
OBJECTS = $(SOURCES:.cpp=.o)

HEADERS = socket.h

all: $(OBJECTS)

%.cpp.o: $(SOURCES)
	$(CXX) -c $(CXXFLAGS) -o $^ $<

$(OBJECTS): $(HEADERS)
