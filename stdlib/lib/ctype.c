#include <ctype.h>

int isalnum(int character) {
    return isalpha(character) || isdigit(character);
}

int isalpha(int character) {
    return islower(character) || isupper(character);
}

int isblank(int character) {
    return character == ' ' || character == '\t';
}

int iscntrl(int character) {
    return (character >= 0 && character <= 31) || character == 127;
}

int isdigit(int character) {
    return character >= '0' && character <= '9';
}

int isgraph(int character) {
    return character >= '!' && character <= '~';
}

int islower(int character) {
    return character >= 'a' && character <= 'z';
}

int isprint(int character) {
    return character >= ' ' && character <= '~';
}

int ispunct(int character) {
    return isgraph(character) && !isalnum(character);
}

int isspace(int character) {
    return character == ' ' || character == '\t' || character == '\n'
            || character == '\r' || character == '\v' || character == '\f';
}

int isupper(int character) {
    return character >= 'A' && character <= 'Z';
}

int isxdigit(int character) {
    return isdigit(character) || (character >= 'a' && character <= 'f')
            || (character >= 'A' && character <= 'F');
}

int tolower(int character) {
    return isupper(character) ? character + ('a' - 'A') : character;
}

int toupper(int character) {
    return islower(character) ? character - ('a' - 'A') : character;
}
