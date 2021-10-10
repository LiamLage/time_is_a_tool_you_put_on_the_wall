/* Author: Liam Lage
 * 09/10/2021
 * Binary Clock */
#if defined(__unix__) || defined(unix) || defined(__unix) || defined(__CYWIN__)
#include <sys/time.h>
#elif defined(__WIN32) || defined(__WINDOWS__)
#include <windows.h>
#endif
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <strings.h>
#include <time.h>
#define clrscr() printf("\e[1;1H\e[2J")

int32_t checktime(void);
int32_t mytime(int32_t interval, int32_t currentMillis, int32_t previousMillis);
int32_t decimalAssist(void);
long tobinary(int);
uint32_t toseconds();
int32_t toHex();
float topercent();
float todegrees();
void todsm(float);
void reverse(char *str);
void tomorse(int32_t dec, char *morse);



int32_t hour, minute, second;

int32_t main(int32_t *argc, char **argv) {
    int32_t interval = 1000;
    int32_t currentMillis, previousMillis;
    currentMillis = previousMillis = 0;
    extern int32_t hour, minute, second;
    

    checktime();
    mytime(interval, currentMillis, previousMillis);
}

int32_t mytime(int32_t interval, int32_t currentMillis, int32_t previousMillis) {
    extern int32_t hour, minute, second;
    int32_t i;
    float deg;
    currentMillis = clock();
        while(1) {
            currentMillis = clock();
            if (currentMillis - previousMillis >= interval) {
            if (hour > 24 || minute > 60 || second > 60) { 
                printf("ERROR");
                exit(0);
            }
            second++;
            clrscr();
            decimalAssist();
            toseconds();
            if (second > 59) {
                minute++;
                second = 0;
                checktime();
            } if (minute > 59) {
                hour++;
                minute = 0;
            } if (hour > 24) {
                second = 0;
                minute = 0;
                hour   = 0;
            }
            deg = todegrees();
            printf("\n%02d: %02d: %02d\n", hour, minute, second);
            toHex();
            printf("\n%9d%c\n", toseconds(), 's');
            printf("%9.4f%%\n", topercent());
            printf("%9.4f%s\n", deg, "°");
            todsm(deg);
            previousMillis = currentMillis;
        }
        
    }
    return 1;
}
/* checktime: make sure time matches 
 * system time */
int32_t checktime(void) {
    extern int32_t hour, minute;
    
    SYSTEMTIME st, lt;
    GetSystemTime(&st);
    GetLocalTime(&lt);
    
    if (hour != lt.wHour || minute != lt.wMinute) {
        hour   = lt.wHour;
        minute = lt.wMinute;
        return 1;
    }
    return 0;
}
// splitting time into individual digits
int32_t decimalAssist() {
    extern int32_t hour, minute, second;
    int32_t dec_hour, dec_minute, dec_second;
    char morseH[6], morseh[6], morseM[6], morsem[6], morseS[6], morses[6];
    int32_t H,h, M,m, S,s;
    H = h = M = m = S = s = 0;
    while (1) {
        dec_hour    = hour;
        dec_minute  = minute;
        dec_second  = second;

        H = dec_hour / 10;
        h = dec_hour % 10;

        M = dec_minute / 10;
        m = dec_minute % 10;

        S = dec_second / 10;
        s = dec_second % 10;

        printf("\n%d\n%d\n%d\n%d\n%d\n%d\n\n", tobinary(H), tobinary(h), tobinary(M), tobinary(m), tobinary(S), tobinary(s));
        
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


int32_t toHex() {
    int32_t dec_h, dec_m, dec_s;
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
    printf("%2s: %2s: %2s \n", hexh, hexm, hexs);
}

uint32_t toseconds() {
    extern int32_t hour, minute, second;
    int32_t hour_local, min, sec;
    uint32_t total_seconds = 0;
    total_seconds = ((hour * 3600) + (minute * 60) + second);
    //printf("TS: %d\n", total_seconds);
    return total_seconds;
}

float topercent() {
    float percent = 0;
    float sec = toseconds();
    percent = (sec / 86400.0) * 100.0;
    return percent;
}

float todegrees() {
    float degrees = 0;
    float sec = toseconds();
    degrees = (sec / 86400) * 360;
    return degrees;
}
void todsm(float dec) {
    double d, m, s; 
    double fractional, rem, rem1;
    fractional = modf(dec, &d);
    rem = modf((fractional*60), &m);
    rem1 = modf(rem*60, &s);
    printf("%3.0f%s%2.0f\'%2.0f\"\n", d, "°", m, s);
}

void reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

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
