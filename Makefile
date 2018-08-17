CXX           = gcc
FLAGS         = -Wall -Wextra -static -pthread
RELEASE_FLAGS = $(FLAGS) -O3 -DNDEBUG -flto
DEBUG_FLAGS   = $(FLAGS) -g -gdwarf-2 -Wall -Wextra

LINKER  = gcc
LFLAGS  = -pthread

EXEC    = baislicka
SRCDIR  = src
OBJDIR  = obj
BINDIR  = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

$(BINDIR)/$(EXEC): $(BINDIR) $(OBJDIR) $(OBJECTS)
	@$(LINKER) -o $@ $(OBJECTS) $(LFLAGS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CXX) $(FLAGS) -c $< -o $@

release:
	$(MAKE) FLAGS="$(RELEASE_FLAGS)"

debug:
	$(MAKE) FLAGS="$(DEBUG_FLAGS)" EXEC="$(EXEC)-debug"

bin:
	mkdir -p $(BINDIR)

obj:
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJECTS)

.PHONY: clean
