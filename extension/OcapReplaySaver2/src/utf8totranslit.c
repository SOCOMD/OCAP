
#include <string.h>
#include <stdlib.h>
#include "utf8totranslit.h"

static int utf8toTranslit_checkAllowed(const char* in_buff) {
    static const char* allowed = "QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm1234567890-_";
    int allowed_symbols = 0;

    while (*in_buff) {
        const char* a = allowed;
        while (*a) {
            if (*a == *in_buff) {
                goto next;
            }
            a++;
        }
        return allowed_symbols;
    next:
        allowed_symbols++;
        ++in_buff;
    }
    return allowed_symbols;
}


int utf8to_translit(const char* in_buff, char* out_buff) {
    static const char* symbols[][2] = {
        {"\xd0\x99","Y"}, /* É */
        {"\xd0\xb9","y"}, /* é */
        {"\xd0\xa6","Ts"}, /* Ö */
        {"\xd1\x86","ts"}, /* ö */
        {"\xd0\xa3","U"}, /* Ó */
        {"\xd1\x83","u"}, /* ó */
        {"\xd0\x9a","K"}, /* Ê */
        {"\xd0\xba","k"}, /* ê */
        {"\xd0\x95","E"}, /* Å */
        {"\xd0\xb5","e"}, /* å */
        {"\xd0\x9d","N"}, /* Í */
        {"\xd0\xbd","n"}, /* í */
        {"\xd0\x93","G"}, /* Ã */
        {"\xd0\xb3","g"}, /* ã */
        {"\xd0\xa8","Sh"}, /* Ø */
        {"\xd1\x88","sh"}, /* ø */
        {"\xd0\xa9","Shch"}, /* Ù */
        {"\xd1\x89","shch"}, /* ù */
        {"\xd0\x97","Z"}, /* Ç */
        {"\xd0\xb7","z"}, /* ç */
        {"\xd0\xa5","Kh"}, /* Õ */
        {"\xd1\x85","kh"}, /* õ */
        {"\xd0\xaa",""}, /* Ú */
        {"\xd1\x8a",""}, /* ú */
        {"\xd0\x81","E"}, /* ¨ */
        {"\xd1\x91","e"}, /* ¸ */
        {"\xd0\xa4","F"}, /* Ô */
        {"\xd1\x84","f"}, /* ô */
        {"\xd0\xab","Y"}, /* Û */
        {"\xd1\x8b","y"}, /* û */
        {"\xd0\x92","V"}, /* Â */
        {"\xd0\xb2","v"}, /* â */
        {"\xd0\x90","A"}, /* À */
        {"\xd0\xb0","a"}, /* à */
        {"\xd0\x9f","P"}, /* Ï */
        {"\xd0\xbf","p"}, /* ï */
        {"\xd0\xa0","R"}, /* Ð */
        {"\xd1\x80","r"}, /* ð */
        {"\xd0\x9e","O"}, /* Î */
        {"\xd0\xbe","o"}, /* î */
        {"\xd0\x9b","L"}, /* Ë */
        {"\xd0\xbb","l"}, /* ë */
        {"\xd0\x94","D"}, /* Ä */
        {"\xd0\xb4","d"}, /* ä */
        {"\xd0\x96","Zh"}, /* Æ */
        {"\xd0\xb6","zh"}, /* æ */
        {"\xd0\xad","E"}, /* Ý */
        {"\xd1\x8d","e"}, /* ý */
        {"\xd0\xaf","Ya"}, /* ß */
        {"\xd1\x8f","ya"}, /* ÿ */
        {"\xd0\xa7","Ch"}, /* × */
        {"\xd1\x87","ch"}, /* ÷ */
        {"\xd0\xa1","S"}, /* Ñ */
        {"\xd1\x81","s"}, /* ñ */
        {"\xd0\x9c","M"}, /* Ì */
        {"\xd0\xbc","m"}, /* ì */
        {"\xd0\x98","I"}, /* È */
        {"\xd0\xb8","i"}, /* è */
        {"\xd0\xa2","T"}, /* Ò */
        {"\xd1\x82","t"}, /* ò */
        {"\xd0\xac",""}, /* Ü */
        {"\xd1\x8c",""}, /* ü */
        {"\xd0\x91","B"}, /* Á */
        {"\xd0\xb1","b"}, /* á */
        {"\xd0\xae","Yu"}, /* Þ */
        {"\xd1\x8e","yu"} /* þ */
    };

    if (!out_buff) {
        return strlen(in_buff) * 4L;
    }

    const size_t symb_size = sizeof(symbols) / sizeof(symbols[0]);

    while (*in_buff) {
        int allowed = utf8toTranslit_checkAllowed(in_buff);
        memcpy(out_buff, in_buff, allowed); out_buff += allowed; in_buff += allowed;
        if (*in_buff == '\x20') {
            *out_buff++ = '_';
            goto next;
        }

        if (!*(in_buff + 1)) {
            goto next;
        }
        for (int i = 0; i < symb_size; ++i) {
            if (*in_buff == symbols[i][0][0] && *(in_buff + 1) == symbols[i][0][1]) {
                size_t repl_len = strlen(symbols[i][1]);
                memcpy(out_buff, symbols[i][1], repl_len);
                out_buff += repl_len;

                ++in_buff; goto next;
            }
        }
    next:
        ++in_buff;
    }
    *out_buff = '\0';
    return 0;
}
