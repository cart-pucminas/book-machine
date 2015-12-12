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

# Directories
BINDIR = bin
INCDIR = include
SRCDIR = src

# Source files.
SRC = $(wildcard $(SRCDIR)/*.c)

# Executable file.
EXEC = book-machine

# Toolchain.
CC = gcc

# Toolchain configuration.
CFLAGS  = -Wall -Wextra
CFLAGS += -pedantic -std=c99
CFLAGS += -I $(INCDIR)

# Builds everything.
all:
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BINDIR)/$(EXEC)
	
# Cleans compilation files.
clean:
	@rm -rf $(BINDIR)/$(EXEC)
