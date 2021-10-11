/* Author: Liam Lage
 * 09/10/2021
 *
 * Time is a tool you can put on the wall 
 * or wear it on your wrist */

#if defined(__unix__) || defined(unix) || defined(__unix) || defined(__CYWIN__)
#include <sys/time.h>
#elif defined(__WIN32) || defined(__WINDOWS__)
#include <windows.h>
#include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#define clrscr() printf("\e[1;1H\e[2J")

// function declaration
void mytime(int32_t interval, int32_t currentMillis, int32_t previousMillis);
int32_t checktime(void);
void decimalAssist(void);
long tobinary(int);
void tomorse(int32_t dec, char *morse);
void toHex(void);
void reverse(char *str);
double tometric(void);
int32_t toseconds(void);
float topercent(void);
float todegrees(void);
void todms(float);
int32_t millis(void);

// global time
int32_t hour, minute, second;

int32_t main(int32_t *argc, char **argv) {
    // timing
    int32_t interval = 999;
    int32_t currentMillis, previousMillis;
    currentMillis = previousMillis = 0;

    // set current time
    checktime();
    
    // keep track of time, control & print other time functions
    mytime(interval, currentMillis, previousMillis);
}



// mytime: keep track of time, control & print other time functions
void mytime(int32_t interval, int32_t currentMillis, int32_t previousMillis) {
    extern int32_t hour, minute, second;    // global time
    char *day;  // day string
    day = "MON";
    float deg;

    // windows time
    SYSTEMTIME st, lt;
    GetSystemTime(&st);
    GetLocalTime(&lt);
    
    currentMillis = clock();
        while(1) {
            printf(" %02d:%02d:%02d.%03d\r", hour, minute, second, millis());    // print global time with milliseconds appended
            // 1 second interval
            currentMillis = clock();
            if (currentMillis - previousMillis > interval) {
            
            // global time out of bounds
            if (hour > 24 || minute > 60 || second > 60) { 
                printf("ERROR");
                exit(0);
            }
            second++;
            // clear screen for update
            //system("cls");
            //syetem("clear");
            clrscr();
            decimalAssist();    // global time split into individual digits, print binary & morse
            toseconds();        // convert global time to seconds & print

            // global timekeeper
            if (second > 59) { 
                minute++;
                second = 0;
            } if (minute > 59) {
                hour++;
                minute = 0;
                checktime();    // once an hour make sure global time matches system time
            } if (hour > 24) {
                second = 0;
                minute = 0;
                hour   = 0;


                // convert SYSTEMTIME day to string
                if (lt.wDayOfWeek == 0) {
                    day = "SUN";
                } else if (lt.wDayOfWeek == 1) {
                    day = "MON";
                } else if (lt.wDayOfWeek == 2) {
                    day = "TUE";
                } else if (lt.wDayOfWeek == 3) {
                    day = "WED";
                } else if (lt.wDayOfWeek == 4) {
                    day = "THU";
                } else if (lt.wDayOfWeek == 5) {
                    day = "FRI";
                } else {
                    day = "SAT";
                }
            }
            deg = todegrees();
            printf("\n%2d/%2d/%2d - %s\n", lt.wDay, lt.wMonth, lt.wYear, day);  // print the date
            printf("  %02d:%02d:%02d\n", hour, minute, second);                 // print 24hr std time
            toHex();                                                            // print hexadecimal time    
            printf("%10.8f", tometric());                                       // print metric time
            printf("\n%9d%c\n", toseconds(), 's');                              // print time in seconds
            printf("%9.4f%%\n", topercent());                                   // print percentage time
            printf("%9.4f%s\n", deg, "°");                                      // print decimal degree time
            todms(deg);                                                         // print DMS time
            previousMillis = currentMillis;
        }
        
    }
}



/* checktime: make sure time matches 
 * system time */
int32_t checktime(void) {
    extern int32_t hour, minute, second;
    
    SYSTEMTIME st, lt;
    GetSystemTime(&st);
    GetLocalTime(&lt);
    
    if (hour != lt.wHour || minute != lt.wMinute) {
        hour   = lt.wHour;
        minute = lt.wMinute;
        second = lt.wSecond;
        return 1;
    }
    return 0;
}



/* decimalAssist: splitting time into individual digits,
 * print those digits in binary and morse code. */
void decimalAssist(void) {
    extern int32_t hour, minute, second;
    int32_t dec_hour, dec_minute, dec_second;
    char morseH[6], morseh[6], morseM[6], morsem[6], morseS[6], morses[6];
    int32_t H,h, M,m, S,s;
    H = h = M = m = S = s = 0;
    while (1) {
        // local copy of global time
        dec_hour    = hour;
        dec_minute  = minute;
        dec_second  = second;

        // split time into individual digits
        H = dec_hour / 10;
        h = dec_hour % 10;

        M = dec_minute / 10;
        m = dec_minute % 10;

        S = dec_second / 10;
        s = dec_second % 10;

        // print the binary representation of the individual digits
        printf("%ld\n%ld\n%ld\n%ld\n%ld\n%ld\n\n", tobinary(H), tobinary(h), tobinary(M), tobinary(m), tobinary(S), tobinary(s));
        
        // print the morse code representation of the individual digits
        tomorse(H, morseH);
        printf(" ");
        tomorse(h, morseh);
        printf("|");
        tomorse(M, morseM);
        printf(" ");
        tomorse(m, morsem);
        printf("|");
        tomorse(S, morseS);
        printf(" ");
        tomorse(s, morses);
        printf("\n");
        
        break;
    }
    
}



// tobinary: convert int to binary
long tobinary(int dec) {
    long bin = 0, rem, i = 1;
    while (dec != 0) {
        rem = dec % 2;
        bin = bin + rem * i;
        i = i * 10;
        dec = dec / 2;
    }
    return bin;
}



// tomorse: convert int to morse code
void tomorse(int32_t dec, char *morse) {
    if (dec == 0) 
        morse = "-----\0";
    else if (dec == 1)
        morse = ".----\0";
    else if (dec == 2)
        morse = "..---\0";
    else if (dec == 3)
        morse = "...--\0";
    else if (dec == 4)
        morse = "....-\0";
    else if (dec == 5)
        morse = ".....\0";
    else if (dec == 6)
        morse = "-....\0";
    else if (dec == 7)
        morse = "--...\0";
    else if (dec == 8)
        morse = "---..\0";
    else if (dec == 9)
        morse = "----.\0";
    printf("%s", morse);
}



// toHex: convert global time to Hexadecimal time and print
void toHex(void) {
    int32_t dec_h, dec_m, dec_s;
    // local copy of global time
    dec_h = hour;
    dec_m = minute;
    dec_s = second;    
    
    int32_t rem, i, j, k; 
    i = j = k = 0;
    char hexh[10];
    char hexm[10];
    char hexs[10]; 
    for (int i = 0; i <= 9; i++) {
        hexh[i] = '\0';
        hexm[i] = '\0';
        hexs[i] = '\0';
    }
    while (dec_h != 0) {
        rem = dec_h % 16;
        if (rem < 10) { 
            hexh[i] = rem + 48;
            i++;
        } else { 
            hexh[i] = rem + 55;
            i++;
        }
        dec_h = dec_h / 16;
    }
    while (dec_m != 0) {
        rem = dec_m % 16;
        if (rem < 10) { 
            hexm[j] = rem + 48;
            j++;
        } else {  
            hexm[j] = rem + 55;
            j++;
        }
        dec_m = dec_m / 16;
    }
    while (dec_s != 0) {
        rem = dec_s % 16;
        if (rem < 10) {
            hexs[k] = rem + 48;
            k++;
        } else {
            hexs[k] = rem + 55;
            k++;
        }
        dec_s = dec_s / 16;
    }
    reverse(hexh);
    reverse(hexm);
    reverse(hexs);
    printf("  %2s:%2s:%2s \n", hexh, hexm, hexs);
}



/* reverse: reverse a string, used to reverse
 * hexadecimal time array */
void reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}



// tometric: convert seconds to base ten time
double tometric(void) {
    int32_t s1 = toseconds();
    double s = (double)s1;
    double metric = s/8640.0;
    return metric; 
}



// toseconds: convert global time to seconds
int32_t toseconds(void) {
    extern int32_t hour, minute, second;
    int32_t total_seconds = 0;
    total_seconds = ((hour * 3600) + (minute * 60) + second);
    //printf("TS: %d\n", total_seconds);
    return total_seconds;
}



// topercent: convert seconds to percent of day
float topercent(void) {
    float percent = 0;
    float sec = toseconds();
    percent = (sec / 86400.0) * 100.0;
    return percent;
}



// todegrees: convert seconds to decimal degrees
float todegrees(void) {
    float degrees = 0;
    float sec = toseconds();
    degrees = (sec / 86400) * 360;
    return degrees;
}



// todms: convert decimal degrees to DMS
void todms(float dec) {
    double d, m, s; 
    double fractional, rem, rem1;
    fractional = modf(dec, &d);
    rem = modf((fractional*60), &m);
    rem1 = modf(rem*60, &s);
    printf("%3.0f%s%2.0f\'%2.0f\"\n", d, "°", m, s);
}



// millis: return milliseconds from system time
int32_t millis(void) {
    int32_t millis;
    SYSTEMTIME st, lt;
    GetSystemTime(&st);
    GetLocalTime(&lt);
    millis = lt.wMilliseconds;
    return millis;
}