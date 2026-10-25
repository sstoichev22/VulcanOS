#ifndef VULCAN_CTYPE_H
#define VULCAN_CTYPE_H

/** Character functions accept EOF or a value representable as unsigned char. */
int isalnum(int character);
int isalpha(int character);
int isblank(int character);
int iscntrl(int character);
int isdigit(int character);
int isgraph(int character);
int islower(int character);
int isprint(int character);
int ispunct(int character);
int isspace(int character);
int isupper(int character);
int isxdigit(int character);

/** Convert ASCII letter case; return every other input value unchanged. */
int tolower(int character);
int toupper(int character);

#endif
