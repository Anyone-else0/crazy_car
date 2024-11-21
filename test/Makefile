CC=gcc

CFLAGS=-I. -I../lib -I../schedule
CFLAGS+=-g
CFLAGS+=-Wall
CFLAGS+=-Werror
CFLAGS+=-Wextra
CFLAGS+=-DCAR_UT

SRCDIR=.
OBJDIR = obj
TARGET=car_ut

C_SOURCES=  \
ut.c \
car_stub.c \
../schedule/car_co.c \
car_co_ut.c

OBJECTS = $(addprefix $(OBJDIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

all:$(OBJDIR) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(TARGET)

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm -rf obj $(TARGET)
