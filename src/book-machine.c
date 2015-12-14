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
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

#include <book-machine.h>

/**
 * @brief Booking.
 */
struct booking
{
	int id;         /* Booking ID.   */
	uid_t uid;      /* User ID.      */
	struct tm date; /* Booking date. */
};

/**
 * @brief Bookings filename.
 */
static const char *bookings = "bookings";

/**
 * @brief Gets next night.
 * 
 * @param curr Current date.
 * 
 * @return Date of next night.
 */
static struct tm next_night(struct tm curr)
{
	time_t t;
	
	/* Skip to next day. */
	curr.tm_mday++;
	
	/* Reset some fields. */
	curr.tm_sec = 0;
	curr.tm_min = 0;
	curr.tm_hour = 18;
		
	t = mktime(&curr);
	curr = *localtime(&t);
	
	return (curr);
}

/**
 * @brief Books this machine.
 */
void book_machine(void)
{
	time_t t;            /* Current time.    */
	struct booking curr; /* Current booking. */
	struct booking next; /* Next booking.    */
	FILE *file;          /* Bookings file.   */
	
	/* Initialize next booking. */
	next.id = 0;
	next.uid = getuid();
	t = time(NULL);
	next.date = next_night(*localtime(&t));
	
	/* Open bookings file. */
	if ((file = fopen(bookings, "r+")) == NULL)
	{
		fprintf(stderr, "failed to open bookings file\n");
		exit(EXIT_FAILURE);
	}
	
	/* Insert booking. */
	for (int i = 0; fread(&curr, sizeof(struct booking), 1, file) == 1; i++)
	{
		next.date = next_night(curr.date);
		next.id = curr.id + 1;
	}

	fwrite(&next, sizeof(struct booking), 1, file);
	
	/* House keeping. */
	fclose(file);
}

/**
 * @brief List current bookings.
 */
void list_bookings(void)
{
	struct booking curr; /* Current booking. */
	FILE *file;          /* Bookings file.   */
	
	/* Open bookings file. */
	if ((file = fopen(bookings, "r")) == NULL)
	{
		fprintf(stderr, "failed to open bookings file\n");
		exit(EXIT_FAILURE);
	}
	
	/* List current bookings. */
	printf("%s %-10s %s \n", "id", "date", "username");
	for (int i = 0; fread(&curr, sizeof(struct booking), 1, file) == 1; i++)
	{
		struct passwd *pw;
		
		pw = getpwuid(curr.uid);
		
		printf("%02d %4d-%02d-%02d %s \n",
			curr.id,                  /* Booking ID. */
			curr.date.tm_year + 1900, /* Year.       */
			curr.date.tm_mon + 1,     /* Month.      */
			curr.date.tm_mday,        /* Day.        */
			pw->pw_name               /* User name.  */
		);
	}
	
	/* House keeping. */
	fclose(file);
}

/**
 * @brief Cancels a booking.
 *
 * @param id ID of targeting booking.
 */
void cancel_booking(int id)
{
	struct booking curr; /* Current booking. */
	FILE *file;          /* Bookings file.   */
	FILE *tmp;           /* Temporary file.  */

	/* Open bookings file. */
	if ((file = fopen(bookings, "r+")) == NULL)
	{
		fprintf(stderr, "failed to open bookings file\n");
		exit(EXIT_FAILURE);
	}

	/* Open temporary file. */
	if ((tmp = tmpfile()) == NULL)
	{
		fprintf(stderr, "failed to open temporary file\n");
		exit(EXIT_FAILURE);
	}
	
	/* Copy bookings to temporary file. */
	for (int i = 0; fread(&curr, sizeof(struct booking), 1, file) == 1; i++)
	{
		/* Skip this booking. */
		if (curr.id == id)
			continue;

		fwrite(&curr, sizeof(struct booking), 1, tmp);
	}

	/* Truncate bookings file. */
	if ((file = freopen(bookings, "w+", file)) == NULL)
	{
		fprintf(stderr, "failed to truncate bookings file\n");
		exit(EXIT_FAILURE);
	}

	/* Copy bookings back to bookings file. */
	rewind(tmp);
	for (int i = 0; fread(&curr, sizeof(struct booking), 1, tmp) == 1; i++)
		fwrite(&curr, sizeof(struct booking), 1, file);
	
	/* House keeping. */
	fclose(tmp);
	fclose(file);
}

/**
 * @brief Updates booking file.
 */
void update_bookings(void)
{
	time_t t;            /* Current time.   */
	struct tm curr;      /* Current time.   */
	struct booking next; /* Next booking.   */
	FILE *file;          /* Bookings file.  */
	FILE *tmp;           /* Temporary file. */
	
	/* Get current time. */
	t = time(NULL);
	curr = *localtime(&t);

	/* Open bookings file. */
	if ((file = fopen(bookings, "r+")) == NULL)
	{
		fprintf(stderr, "failed to open bookings file\n");
		exit(EXIT_FAILURE);
	}

	/* Open temporary file. */
	if ((tmp = tmpfile()) == NULL)
	{
		fprintf(stderr, "failed to open temporary file\n");
		exit(EXIT_FAILURE);
	}
	
	/* Copy bookings to temporary file. */
	for (int i = 0; fread(&next, sizeof(struct booking), 1, file) == 1; i++)
	{
		/* Skip this booking. */
		if (curr.tm_year > next.date.tm_year)
			continue;
		else if (curr.tm_year == next.date.tm_year)
		{
			if (curr.tm_yday > next.date.tm_yday)
				continue;
			else if (curr.tm_yday == next.date.tm_yday)
				printf("booking on board\n");
		}

		fwrite(&next, sizeof(struct booking), 1, tmp);
	}

	/* Truncate bookings file. */
	if ((file = freopen(bookings, "w+", file)) == NULL)
	{
		fprintf(stderr, "failed to truncate bookings file\n");
		exit(EXIT_FAILURE);
	}

	/* Copy bookings back to bookings file. */
	rewind(tmp);
	for (int i = 0; fread(&next, sizeof(struct booking), 1, tmp) == 1; i++)
		fwrite(&next, sizeof(struct booking), 1, file);
	
	/* House keeping. */
	fclose(tmp);
	fclose(file);
}
