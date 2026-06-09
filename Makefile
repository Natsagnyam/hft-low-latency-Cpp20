# Compiler Settings
CC = gcc
CFLAGS = -std=c18 -O3 -g -march=native -Wall -Iinclude
TSAN_FLAGS = -fsanitize=thread
LDFLAGS = -pthread

# Directory Definitions
SRCDIR = src
OBJDIR = obj
TARGET = hft_engine

# Instrumented source files (TSAN enabled)
TSAN_SRCS = $(SRCDIR)/main.c $(SRCDIR)/packet_pipeline.c $(SRCDIR)/rt_scheduler.c $(SRCDIR)/latency_logger.c
TSAN_OBJS = $(TSAN_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Non-instrumented source files (Hardware access, TSAN disabled)
HW_SRCS = $(SRCDIR)/pci_discovery.c
HW_OBJS = $(HW_SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(TARGET)

# Debug build: Apply TSAN flags for instrumentation
debug-tsan: LDFLAGS += $(TSAN_FLAGS)
debug-tsan: $(TARGET)

$(TARGET): $(TSAN_OBJS) $(HW_OBJS)
	$(CC) $(TSAN_OBJS) $(HW_OBJS) -o $@ $(LDFLAGS)

# Rule for instrumented objects (TSAN enabled)
$(TSAN_OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) $(TSAN_FLAGS) -c $< -o $@

# Rule for non-instrumented objects (TSAN disabled)
$(HW_OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean debug-tsan