#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sysinfoapi.h>
#define clrscr() printf("\e[1;1H\e[2J")

void wordAssist(void);
void convert(char *num);
int str_len(char s[]);
void copy_str(char from[], char to[]);


int main (void) {
    int currentMillis = 0, previousMillis = 0, interval = 1000;
    while(1) {
        currentMillis = clock();
        if (currentMillis - previousMillis > interval) { 
            clrscr();
            wordAssist();
            previousMillis = currentMillis;
        }
    }    
}


void wordAssist(void) {
    // windows time
    SYSTEMTIME st, lt;
    GetSystemTime(&st);
    GetLocalTime(&lt);


    int hour_local   = lt.wHour;
    int minute_local = lt.wMinute;
    int second_local = lt.wSecond;
    char hr[3];
    char x[3];
    char mn[3];
    char y[3];
    char s[3];
    char z[3];
    int i, j, k;
    i = j = k = 0;

    sprintf(x, "%d", hour_local);
    sprintf(y, "%d", minute_local);
    sprintf(z, "%d", second_local);
    
    while ((x[i]) != 0)  {
        hr[i] = x[i];
        i++;
    }
    hr[i] = '\0';   // NULL terminate
    while ((y[j]) != 0) {
        mn[j] = y[j];
        j++;
    }
    mn[j] = '\0';
    while ((z[k]) != 0) {
        s[k] = z[k];
        k++;
    }
    s[k] = '\0';

    printf("the time is ");
    convert(hr);
    printf(" hours, ");
    convert(mn);
    printf(" minutes and ");
    convert(s);
    printf(" seconds\n");
}



void convert(char *num) {

   int len = str_len(num);
   // cases
   if (len == 0) {
      fprintf(stderr, "empty string\n");
      return;
   }
   if (len > 4) {
      fprintf(stderr, "Length more than 4 is not supported\n");
      return;
   }
   // the first string wont be used.
   char *single_digit[] = { "zero", "one", "two", "three", "four","five", "six", "seven", "eight", "nine"};
   // The first string is not used, it is to make
   // array indexing simple
      char *tens_place[] = {"", "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
   // The first two string are not used, they are to make
   // array indexing simple
      char *tens_multiple[] = {"", "", "twenty", "thirty", "forty", "fifty","sixty", "seventy", "eighty", "ninety"};
      char *tens_power[] = {"hundred", "thousand"};
   // Used for debugging purpose only
   //printf("\n%s: ", num);

   // For single digit number
   if (len == 1) {
      printf("%s", single_digit[*num - '0']);
      return;
   }
   // Iterate while num is not '\0'
   while (*num != '\0') {
      // Code path for first 2 digits
      if (len >= 3) {
         if (*num -'0' != 0) {
            printf("%s", single_digit[*num - '0']);
            printf("%s", tens_power[len-3]); // here len can be 3 or 4
         }
         --len;
      }
      // Code path for last 2 digits
      else {
         // Need to explicitly handle 10-19. Sum of the two digits is
         //used as index of "tens_place" array of strings
         if (*num == '1') {
            int sum = *num - '0' + *(num + 1)- '0';
            printf("%s", tens_place[sum]);
            return;
         }
         // Need to explicitely handle 20
         else if (*num == '2' && *(num + 1) == '0') {
            printf("twenty");
            return;
         }
         // Rest of the two digit numbers i.e., 21 to 99
         else {
            int i = *num - '0';
            printf("%s ", i? tens_multiple[i]: "");
            ++num;
            if (*num != '0')
               printf("%s", single_digit[*num - '0']);
         }
      }
      ++num;
   }
}

int str_len(char s[])   {
    int offset;
    while ((s[offset]) != 0)
        offset++;
    return offset;
}

// copy a string 
void copy_str(char from[], char to[])   {
    int i;

    while ((from[i]) != 0)  {
        to[i] = from[i];
        i++;
    }
    to[i] = '\0';   // NULL terminate
}