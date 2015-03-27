CXXFLAGS += -std=c++11
CXXFLAGS += -Wall -Werror

hanoi: hanoi.cpp
	$(CXX) $(CXXFLAGS) hanoi.cpp -o hanoi

clean:
	rm -f hanoi
