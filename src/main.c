/*
 * Copyright(C) 2015 Pedro H. Penna <pedrohenriquepenna@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <book-machine.h>

/**
 * @brief Program options
 */
typedef enum {
	BOOK,   /**< Books this machine.    */
	CANCEL, /**< Cancels a booking.     */
	LIST,   /**< List current bookings. */
	UPDATE  /**< Update bookings.       */
} options_type;	

/**
 * @brief Program options.
 */
static options_type option;

/**
 * @brief Booking to cancel.
 */
static int id = -1;

/**
 * @brief Prints program usage and exits.
 */
static void usage(void)
{
	printf("Usage: book-machine [options]\n\n");
	printf("Brief: books this machine for exclusive use\n\n");
	printf("Options:\n");
	printf("    --book        books this machine for the next night\n");
	printf("    --cancel <id> cancels a booking\n");
	printf("    --help        prints this information and exits\n");
	printf("    --list        list current bookings\n");
	printf("    --update      update bookings\n");
	
	exit(EXIT_SUCCESS);
}

/**
 * @brief Gets program arguments.
 * 
 * @param argc Argument count.
 * @param argv Arguments.
 */
static void getargs(int argc, char **argv)
{
	/* Book machine. */
	if (!(strcmp(argv[1], "--book")))
		option = BOOK;
	
	/* Cancels a booking. */
	else if (!(strcmp(argv[1], "--cancel")))
	{
		/* Wrong usage. */
		if (argc < 3)
			usage();
		option = CANCEL;
		id = atoi(argv[2]);
	}
	
	/* List bookings. */
	else if (!(strcmp(argv[1], "--list")))
		option = LIST;
	
	/* Update bookings. */
	else if (!(strcmp(argv[1], "--update")))
		option = UPDATE;
	
	/* Help. */
	else
		usage();
}

/**
 * @brief Books this machine.
 */
int main(int argc, char **argv)
{
	getargs(argc, argv);
	
	switch (option)
	{
		/* Book this machine. */
		case BOOK:
			book_machine();
			break;
		
		/* Cancels a booking. */
		case CANCEL:
			cancel_booking(id);
			break;
		
		/* List current bookings. */
		case LIST:
			list_bookings();
			break;
		
		/* Update bookings. */
		case UPDATE:
			update_bookings();
			break;
	}
	
	return (EXIT_FAILURE);
}
