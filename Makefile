# Makefile

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread
INCLUDES = -Iincludes
SRC = src/rate_limiter/token_bucket/token_bucket.cpp
OUTDIR = bin
TARGET = $(OUTDIR)/rate_limiter_test

# Default rule
all: $(TARGET)

$(TARGET): $(SRC)
	@mkdir -p $(OUTDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean
