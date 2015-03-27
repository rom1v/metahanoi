CXXFLAGS += -std=c++11
CXXFLAGS += -Wall -Werror

all: hanoi metahanoi

hanoi: hanoi.cpp
	$(CXX) $(CXXFLAGS) hanoi.cpp -o hanoi

metahanoi: metahanoi.cpp
	$(CXX) $(CXXFLAGS) metahanoi.cpp -o metahanoi

clean:
	rm -f hanoi metahanoi
