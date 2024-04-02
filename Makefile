CXXFLAGS = -std=c++11
DEBUGFLAGS = -g
LDFLAGS = -lcrypto -lpthread

SRCDIR = src
BUILDDIR = build

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

all: $(BUILDDIR)/prog

debug: CXXFLAGS += $(DEBUGFLAGS)
debug: $(BUILDDIR)/prog

$(BUILDDIR)/prog: $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)

.PHONY: all debug clean
