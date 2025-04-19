CXX = g++
CXXFLAGS = -Werror -Wpedantic -Wall -std=c++17 -Iinclude -I./libs/eigen-3.4.0

BUILDDIR = build
BINDIR = bin
DATADIR = data

TARGET = $(BINDIR)/Start

SRCS_MAIN = Encoder_Shannon.cpp Decoder_Shannon.cpp main.cpp
SRCS_TEST = test.cpp

OBJS_MAIN = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SRCS_MAIN))
OBJS_TEST = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(SRCS_TEST))

GTEST_LIBS = -lgtest -lgtest_main -pthread

all: $(BUILDDIR) $(BINDIR) $(TARGET) $(DATADIR)

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BINDIR):
	@mkdir -p $(BINDIR)

$(DATADIR):
	@mkdir -p $(DATADIR)

$(TARGET): $(OBJS_MAIN)
	@mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(BUILDDIR)/test.o $(OBJS_TEST) | $(BUILDDIR) $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/test $^ $(GTEST_LIBS)
	$(BINDIR)/test

$(BUILDDIR)/test.o: test.cpp | $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@
 
clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(DATADIR)

.PHONY: all clean test
