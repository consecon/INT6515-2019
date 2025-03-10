/* This software was developed at the National Institute of Standards and
* Technology by employees of the Federal Government in the course of their
* official duties. Pursuant to title 17 Section 105 of the United States
* Code this software is not subject to copyright protection and is in the
* public domain. NIST assumes no responsibility whatsoever for its use by
* other parties, and makes no guarantees, expressed or implied, about its
* quality, reliability, or any other characteristic.

* We would appreciate acknowledgement if the software is used.
* The SAMATE project website is: http://samate.nist.gov
*/

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <stdlib.h>
#include <stdint.h>

/* Replacement to memset() that cannot be optimized out */
char *my_memset_s(void *s, int c, size_t n)
{
	volatile uint8_t *p = (uint8_t *)s;
	while (n--)
		*p++ = c;

	return s;
}

/* Reads a password from a terminal - replaces obsolete getpass() function */
char * getpass_r(const char *prompt)
{
	struct termios oflags, nflags;
	char password[64] = { '\0' };
	char * ret = NULL;

	/* Disabling echo */
	if(tcgetattr(fileno(stdin), &oflags))
		return NULL;

	nflags = oflags;
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag |= ECHONL;

	if (tcsetattr(fileno(stdin), TCSAFLUSH, &nflags))
		return NULL;

	/* Read the password */
	fprintf(stderr, "%s", prompt);
	ret = fgets(password, sizeof(password), stdin);

	/* Restore echo */
	if (tcsetattr(fileno(stdin), TCSAFLUSH, &oflags))
	{
		my_memset_s(password, 0, sizeof(password));
		return NULL;
	}

	if(ret)
	{
		strtok(password, "\n");
		ret = strdup(password);
		my_memset_s(password, 0, sizeof(password));
	}

	return ret;
}

void
test(void)
{
        char *password = getpass_r("Password: ");

        if(password != NULL)
	{
                if (strcmp(password, "Mew!"))				/* FLAW */
		{
			my_memset_s(password, 0, strlen(password));
			free(password);
			printf("Incorrect Password!\n");
			return;
		}
		my_memset_s(password, 0, strlen(password));
		free(password);
	}
	printf("Entering Diagnostic Mode...\n");
}
 
int
main(int argc, char **argv)
{
        test();
        return 0;
}


