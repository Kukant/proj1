/**
*   Prvni projekt do IZP, prace s textem.
*   Autor: Tomas Kukan, xkukan00
*   Verze: 1.0
*   Datum: 30. 10. 2016
*
*   Popis funkce programu:
*   Tento program ma nekolik funkci, pri spusteni se da zvolit pouze jedna a to pomoci parametru
*   -x nebo -S N nebo -s S -n N (da se pouzit i pouze jeden, nebo zadny) nebo -r. Pri spusteni s parametrem -x program
*   preklada vstupni text do do hexidecimalni podoby na jeden radek. Pri spusteni s parametrem -S N
*   vypisuje program vstupni retezce pouze takove, ktere jsou delsi nez cislo N (toto cislo musi byt zadano).
*   Pri parametrech -s S -n N nebo bez zadneho program preklada textovy vstup do hexadecimalni podoby s
*   adresou prvniho bajtu 16 bajtove posloupnosti. Pri spusteni s parametrem -r dela program opak funkce -x
*   tudiz preklada hexadecimalni vstup do znakove podoby.
*/

#include <stdio.h> /* zahrnuti zakladni vstupni a vystupni knihovny */

#define TOO_MANY_ARGUMENTS 1

#define WRONG_ARGUMENT 0

/* FUNKCNI PROTOTYPY FUNKCI */

void argument_x();
void argument_nth_s_n(int skip, int num_of_chars);
void argument_S(int min_lenght);
int argument_r();
int str_compare(char *string1, char *string2);
int str_to_int(char *string);
int test_speficicator(char *test);
int test_is_argument(int argv_number, int argc);
void print_hexaprinted_string(long int *string);
void find_assist_numbers(char *string, int *x, int *y);
void manual();
void print_wrong_argument(int mistake_number);

/* FUNKCE MAIN
*
* argv je pole argumentu programu, argc pocet argumentu
*/
int main(int argc, char *argv[])
{

    int is_x = 0, is_s = 0, is_n = 0, is_r = 0, is_S = 0;
    int skip = 0, num_of_chars = 0, min_lenght = 0;

    for(int i = 1; i < argc; i++) /* cyklus pouzity pro zjisteni zadanych argumentu */
    {
        if(str_compare(argv[i], "-x")) /*zacatek testovani argumentu */
        {
            if(is_x == 2)
            {
                print_wrong_argument(TOO_MANY_ARGUMENTS);
                return 1;
            }

            is_x = 2;

            continue;
        }

        else if(str_compare(argv[i], "-s")) /*is_s a is_n ma hodnotu 1 neb muzout byt pouzity zaroven*/
        {
            if(is_s == 1)
            {
                print_wrong_argument(TOO_MANY_ARGUMENTS);
                return 1;
            }

            is_s = 1;

            if(test_is_argument(i+1, argc))
                return 1;

            if(i + 2 <= argc)
            {
                if(test_speficicator(argv[i + 1]))
                    return 0;

                skip = str_to_int(argv[i + 1]);
            }
            else
            {
                print_wrong_argument(WRONG_ARGUMENT);
                manual();
                return 0;
            }

            i++; /* pristi argument muzeme preskocit, neb je to specifikator argumentu -s */
            continue;
        }

        else if(str_compare(argv[i], "-n"))
        {
            if(is_n == 1)
            {
                print_wrong_argument(TOO_MANY_ARGUMENTS);
                return 1;
            }

            is_n = 1;

            if(test_is_argument(i+1, argc))
                return 1;

            if(test_speficicator(argv[i + 1]))
                return 1;

            if(str_to_int(argv[i + 1]) == 0) /*test zdali je n aspon 1 */
                return 0;

            num_of_chars = str_to_int(argv[i + 1]);

            i++; /* pristi argument muzeme preskocit, neb je to specifikator argumentu -n */
            continue;
        }

        else if(str_compare(argv[i], "-S"))
        {
            if(is_S == 2)
            {
                print_wrong_argument(TOO_MANY_ARGUMENTS);
                return 1;
            }

            is_S = 2;

            if(test_is_argument(i+1, argc))
                return 1;

            if(test_speficicator(argv[i + 1]))
                return 0;

            min_lenght = str_to_int(argv[i + 1]);

            if((min_lenght > 199) || (min_lenght < 1)) /*otestovani zdali je argument ve specifikovanem intervalu*/
            {
                print_wrong_argument(TOO_MANY_ARGUMENTS);
                return 1;
            }

            i++; /* pristi argument muzeme preskocit, neb je to specifikator argumentu -S */
            continue;
        }

        else if(str_compare(argv[i], "-r"))
        {
            if(is_r == 2)
            {
                print_wrong_argument(TOO_MANY_ARGUMENTS);
                return 1;
            }

            is_r = 2;
            continue;
        }

        else                                       /*pokud argument nebude ani jeden z platnych argumentu, program se ukonci */
        {
            print_wrong_argument(WRONG_ARGUMENT);
            manual();
            return 1;
        }

    }

    if((is_n + is_r + is_S + is_s + is_x ) > 2) /*pokud bude volan vic jak jeden argument (mimo is_n...)*/
    {
        print_wrong_argument(WRONG_ARGUMENT);
        manual();
        return 1;
    }

    if(is_x)                /*konec testovani argumentu, zacatek volani potrebnych funkci*/
    {
        argument_x();
        return 0;
    }

    if( (is_r == 0 && is_s == 0 && is_n == 0 && is_S == 0 && is_x == 0) || (is_n) || (is_s) )
    {
        argument_nth_s_n(skip, num_of_chars);
        return 0;
    }

    if(is_S)
    {
        argument_S(min_lenght);
        return 0;
    }

    if(is_r)
    {
        if(argument_r())
            return 1;

        else return 0;
    }                                               /*konec volani funkci, nasleduje konec funkce main */
}

/* DEFINICE FUNKCI */

/* Otestuje zda string za urcenym argumentem
* je cele kladne cislo, vrati 1 pokud neni a vypise hlasku,
* 0 pokud je. Argument fce je pointer na urcity argument.
* Pouziti ve funkci main.
*/
int test_speficicator(char *test)
{
    int i = 0;
    while(test[i] != '\0')
    {
        if((test[i] - '0') > 10 || (test[i] - '0') < 0 )
        {
            fprintf(stderr,"Specifikace nektereho parametru neni kladne cele cislo! Po manualu nastane ukonceni programu.\n");
            manual();
            return 1;
        }

        i++;
    }

    return 0;
}

/* Otestuje zdali existuje nejaky retezec za argumentem,
* vypise hlasku pokud tu zadny retezec neexistuje,
* volani u testovani parametru -s -n, -S
*/
int test_is_argument(int argv_number, int argc)
{
    if(argv_number  < argc)
        return 0;
    else
    {
    print_wrong_argument(WRONG_ARGUMENT);
    manual();
    return 1;
    }
}

/* Funkce vrati 0, pokud jsou 2 predane
* retezce stejne, jinak vrati 1
*/
int str_compare(char *string1, char *string2)
{
    int i;

    for(i = 0; (string1[i] == string2[i]) && (string1[i] != '\0') && (string2[i] != '\0'); i++) /*pokud se znamenka nerovnaji, nebo se cyklus dostane na konec jednoho popr. obou slov, tak ukonci cyklus. */
        ;

    if(string1[i] != string2[i]) return 0; /* Pokud se posledni 2 znamenka nactene v cyklu nerovnaji , vrati program 0 */
    else return 1;

}

/* vlastni verze funkce atoi,
* vraci int hodnotu char retezce
*/
int str_to_int(char *string)
{
    int result = 0;

    for(int i = 0; string[i] != '\0'; i++)
    {
        result = 10 * result + string[i] - '0';  /* 10*result posune cislo o jedno doleva, aby vytvoril misto pro dalsi cislo, ktere se pricte */
    }

    return result;
}

/* vytiskne vstup v
* hexadecimalni podobe,
* pouziva stdin
*/
void argument_x() /* vytiskne vstup v hexadecimalni podobe */
{
    long int c; /*kvuli ceskym znakum*/

    while((c = getchar()) != EOF)
    {

        if(c <= 15)
            printf("0");

        printf("%x", (unsigned int) c);
    }

    printf("\n");

    return;
}

/*num_of_chars bude 0, pokud nebyl zadan
* argument -n, jinak bude kladne cislo.
* Funkce vypisuje tex. vstup na hex. vystup, pise adresu
*  1. bajtu a vypisuje 16 bajtu na jeden radek, na konec prida textovou podobu
* techto 16 bajtu.
* pouziva stdin.
*/
void argument_nth_s_n(int skip, int num_of_chars)
{
    long int string_in_use[16];
    long int c;
    long int chars_printed = 0;
    int j = 0;
    int to_write = num_of_chars;

    for(int i = 0; i < skip && c != EOF; i++)  /*cyklus preskoci tolik znaku, kolik je v promenne skip*/
        c = getchar();

    if(c != EOF)
    {
        c = getchar(); /*nacteni prvniho znaku*/

        do
        {
            if((chars_printed % 16) == 0) /*vzdy po vypsani 16b, vypis retezec a vypis adresu prvniho dalsiho bitu */
            {

                if(chars_printed != 0){
                    print_hexaprinted_string(string_in_use);
                    j = 0;
                }

                if(chars_printed != 0) /*poprve nevypise \n */
                    printf("\n%08x  ",(unsigned int) chars_printed + skip);

                else
                   printf("%08x  ",(unsigned int) chars_printed + skip);
            }

            else if((chars_printed % 16) == 8) /*jestlize je vypsanych prave 8b, vypis mezeru navic */
                printf(" ");

            string_in_use[j] = c; /*zapis znak do string_in_use, prave vypisovaneho rezece*/
            j++;

            printf("%02x ",(unsigned int) c);

            chars_printed++;
            to_write--;

        }while( ((c = getchar()) != EOF) && (num_of_chars == 0 || to_write > 0) );

        int printed_on_the_last_line = (chars_printed % 16); /* pomocna promenna pro lepsi pochopeni */

        if(printed_on_the_last_line != 0)
        {
            if((chars_printed % 16) < 9 && printed_on_the_last_line != 0)
                    printf(" ");

            for(int i = 0; i < (16 - printed_on_the_last_line); i++) /*cyklus vypise potrebne mezery, v string_in_use prepise drivejsi text na mezery */
            {
                printf("   ");
                string_in_use[printed_on_the_last_line + i] = ' ';
            }
        }

        print_hexaprinted_string(string_in_use);
    }

	if(chars_printed != 0)
	    printf("\n");
    return;
}

/*vypise string, ktery mu je zadan a ma mensi
* velikost nez 17 B, pouziti u volani argument_nth_s_n
*/
void print_hexaprinted_string(long int *string_in_use)
{
    for(int i = -1; i < 17; i++)
        {
            if(i == -1)
                printf(" |");

            else if(i == 16)
                printf("|");

            else if(string_in_use[i] < 32 || string_in_use[i] > 126) /*testovani zdali je znak tisknutelny */
                printf(".");

            else if(i != -1)
                printf("%c", (int)string_in_use[i]);
        }

    return;
}

/*funkce vypise retezce ze stdin
* ktere jsou delsi nez min_lenght, odratkuje je
*/
void argument_S(int min_lenght) /*program vypise retezce ze stdin ktere jsou delsi nez min_lenght, odratkuje je*/
{
    char string[201]; /* nemuzu deklarovat podle min_lenght neb v dobe prekladu jeste neni znam*/
    char c;
    int i,printed = 0;

    c = getchar();
    while(c != EOF)
    {
        for(i = 0; i < min_lenght && c != EOF; i++)
        {
            if((c > 31 && c < 126) || c == 9) /*testuje zdali je znak tisknutelny nebo mezera nebo tabulator */
                string[i] = c;
            else
                i = -1; /*aby na zacatku dalsiho cyklu bylo i == 0*/

            c = getchar();
        }

        if(i == min_lenght)
        {
            if(printed != 0)
                printf("\n");

            for(int j = 0; j < min_lenght; j++) /*vypis retezec*/
                printf("%c", string[j]);

            printed++;
        }

        while((c > 31 && c < 126 ) || c == 9)
        {
            printf("%c", c);
            c = getchar();
        }


    }

    printf("\n");
    return;
}

/*
* Funkce dela opacnou funkci nez fce s parametrem -x
* ocekava poslouptnost dvouznamenkovych hexa cislic
* preklada je do normalniho textu. Pracuje s stdin.
* Vrati hodnotu 1, pokud je chybny vstup.
*/
int argument_r()
{
    char c = 1, string[2];
    int x,y,result,i;

    while(c != EOF)
    {
            for(i = 0; i < 2 && c != EOF; i++) /*cyklus pro nacteni dvou znaku do pole string*/
            {
                c = getchar();

                if(c == EOF)
                {
                    if(i != 0)      /*vyraz je pravda, pokud je lichy pocet cisel, prehodi cislo o misto doprava, aby byl spravny prevod*/
                    {
                        string[1] = string[0];
                        string[0] = '0';
                    }

                    break;
                }

                if( (c > 47 && c < 58) || (c > 96 && c < 103) || (c > 64 && c < 71) ) /*je c nejaky znak z hexadecimalni soustavy?*/
                    string[i] = c;

                else if((c <= 32 && c >= 0) || c == 127) /*pokud neni znak z hex. soustavy, zkusi dalsi znak*/
                    i--;

                else             /*pokud znak neni bily, ani tisknutelny, ukonci se program s hodnotou 1*/
                {
                    fprintf(stderr, "\nZadali jste znak ktery neni z hex. soustavy! Ukonceni programu.\n");
                    manual();
                    return 1;
                }
            }

            if(c == EOF && i == 0) /*toto nastane pokud bude sudy pocet cisel*/
                    break;

            find_assist_numbers(string, &x, &y);   /* x a y jsou pomocne promenne pro prevod z char na int*/

            result = (string[0] - x) * 16 + string[1] - y; /*prevod hex. na decimalni dle normalniho postupu*/

            printf("%c", result);
    }

    printf("\n");
    return 0;

}

/*funkce vracejici pomocne promenne pro
* prevod z char na int (abychom zjistili jejich decimalni hodnotu),
* pouziti u volani parametru -r
*/
void find_assist_numbers(char *string, int *x, int *y)
{
            if(string[0] > 96 && string[0] < 103)
                *x = 87;

            else if(string[0] > 64 && string[0] < 71)
                *x = 55;

            else
                *x = 48;

            if(string[1] > 96 && string[1] < 103)
                *y = 87;

            else if(string[1] > 64 && string[1] < 71)
                *y = 55;

            else
                *y = 48;
    return ;
}

 /*
 * vypis chyboveho hlaseni
 */
void print_wrong_argument(int mistake_number)
{
    switch(mistake_number)
    {
        case 0: fprintf(stderr, "Zadali jste chybny argument. Po vypsani manualu nastane ukonceni programu.\n");
            break;

        case 1: fprintf(stderr, "Zadali jste prilis mnoho argumentu, nastane ukonceni programu.\n");
            break;
    }

    return ;
}

/*
*vypis manualu
*/
void manual()
{
    fprintf(stderr, "Prvni projekt do IZP, prace s textem.\nAutor:Tomas Kukan, xkukan00 \nVerze: 1.0\n"
    "Popis funkce programu: \nTento program ma nekolik funkci, pri spusteni se da zvolit pouze jedna a to pomoci parametru \n"
    "-x nebo -S N nebo -s S -n N (da se pouzit i pouze jeden, nebo zadny) nebo -r. Pri spusteni s parametrem -x program \n"
    "preklada vstupni text do do hexidecimalni podoby na jeden radek. Pri spusteni s parametrem -S N \n"
    "vypisuje program vstupni retezce pouze takove, ktere jsou delsi nez cislo N (toto cislo musi byt zadano). \n"
    "Pri parametrech -s S -n N nebo bez zadneho program preklada textovy vstup do hexadecimalni podoby s \n"
    "adresou prvniho bajtu 16 bajtove posloupnosti. Pri spusteni s parametrem -r dela program opak funkce -x\n"
    "tudiz preklada hexadecimalni vstup do znakove podoby.\n");

    return ;
}
