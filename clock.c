/* Author: Liam Lage
 * 09/10/2021
 * Binary Clock */
#include <stdio.h>
#include <stdint.h>
#include <strings.h>
#include <time.h>
#include <windows.h>
#define clrscr() printf("\e[1;1H\e[2J")

int32_t checktime(void);
int32_t mytime(int32_t interval, int32_t currentMillis, int32_t previousMillis);
int32_t binaryHelp(void);
long tobinary(int);
uint32_t toseconds();
int32_t toHex();
float topercent();
float todegrees();
void reverse(char *str);



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
            binaryHelp();
            toseconds();
            if (second > 59) {
                minute++;
                second = 0;
            } if (minute > 59) {
                hour++;
                minute = 0;
                checktime();
            } if (hour > 24) {
                second = 0;
                minute = 0;
                hour   = 0;
            }
            printf("\n%02d: %02d: %02d\n", hour, minute, second);
            toHex();
            printf("%8d%c\n", toseconds(), 's');
            printf("%8.4f%%\n", topercent());
            printf("%7.4f%s\n", todegrees(), "°");
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
int32_t binaryHelp() {
    extern int32_t hour, minute, second;
    int32_t dec_hour, dec_minute, dec_second;
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

        printf("\n%d\n%d\n%d\n%d\n%d\n%d\n", tobinary(H), tobinary(h), tobinary(M), tobinary(m), tobinary(S), tobinary(s));
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
    for (int i; i <= 10; i++) {
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
    printf("%02s: %02s: %02s\n", hexh, hexm, hexs);
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

void reverse(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}
