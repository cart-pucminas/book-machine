#
# Copyright(C) 2015 Pedro H. Penna <pedrohenriquepenna@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Installation prefix.
PREFIX = /usr/local

# Directories
BINDIR = bin
INCDIR = include
SRCDIR = src

# Bookings file.
BOOKINGS_FILE=$(PREFIX)/etc/bookings

# Source files.
SRC = $(wildcard $(SRCDIR)/*.c)

# Executable file.
EXEC = book-machine

# Toolchain.
CC = gcc

# Toolchain configuration.
CFLAGS  = -Wall -Wextra
CFLAGS += -pedantic -std=c99
CFLAGS += -I $(INCDIR) -DBOOKINGS_FILE=$(BOOKINGS_FILE)

# Builds everything.
all:
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BINDIR)/$(EXEC)

# Installs book-machine.
install: all
	@touch $(BOOKINGS_FILE)
	@chmod 600 $(BOOKINGS_FILE)
	@chown root:root $(BOOKINGS_FILE) 
	@mkdir -p $(PREFIX)/bin
	@mv $(BINDIR)/$(EXEC) $(PREFIX)/bin/
	@chmod 6555 $(PREFIX)/bin/$(EXEC)
	@chown root:root $(BOOKINGS_FILE)
	@echo "$(PREFIX)/bin/$(EXEC) --update" >> /etc/profile

# Uninstalls book-machine.
uninstall:
	@rm -f $(BOOKINGS_FILE)
	@rm -f $(PREFIX)/bin/$(EXEC)
	@head -n -1 /etc/profile > /etc/profile2
	@rm -f /etc/profile
	@mv /etc/profile2 /etc/profile

# Cleans compilation files.
clean:
	@rm -f $(BINDIR)/$(EXEC)
