#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float myfabs(float x)
{
    if(x<0)
        return x*(-1);
    return x;
}

int main(int argc, char *argv[])
{
    FILE *old, *new;
    float offset; //offset to be added (in seconds)
    if (argc != 3)
    {
        printf("invalid number of arguments.\n");
        exit(1);
    }
    old = fopen(argv[1], "rt");
    if (old == NULL)
    {
        printf("couldn't open file named '%s'\n", argv[1]);
        exit(1);
    }
    new = fopen(argv[2], "wt");
    if (new == NULL)
    {
        printf("couldn't create file named '%s'\n", argv[1]);
        exit(1);
    }

    char lineStr[200], holder[8];

    int hrs, min;
    float sec;
    printf("enter an offset \n_>");
    scanf("%f",&offset);
    if(myfabs(offset)>59.999)
    {
        printf("absolute value of the offsett can't be higher than 59.999 seconds\n");
        exit(1);
    }
    while (fgets(lineStr, 200, old) != NULL)
    {
        //printf("%s",lineStr);
        fprintf(new, "%s", lineStr);
        fgets(lineStr, 200, old);
        //printf("%s",lineStr);


        for (int i = 0; i < 2; i++)
        {
            //getting hours
            memcpy(holder, &lineStr[0+i*17], 2 * sizeof(char));
            holder[2] = '\0';
            hrs = atoi(holder);
            //getting minutes
            memcpy(holder, &lineStr[3+i*17], 2 * sizeof(char));
            holder[2] = '\0';
            min = atoi(holder);
            //getting seconds
            memcpy(holder, &lineStr[6+i*17], 6 * sizeof(char));
            holder[6] = '\0';
            holder[2] = '.';
            sec = atof(holder);
            //adding offset
            sec += offset;
            if (sec > 59.999)
            {
                sec -= 60;
                min++;
                if (min == 60)
                {
                    min = 0;
                    hrs++;
                }
            }
            if (sec < 0)
            {
                if (min == 0 && hrs == 0)
                {
                    sec = 0;
                    min = 0;
                    hrs = 0;
                }
                else
                {
                    sec = 60 + sec;
                    min--;
                    if (min < 0)
                    {
                        min = 59;
                        hrs--;
                    }
                }
            }
            //printing results
            if (hrs < 10)
                fprintf(new, "0%d:", hrs);
            else
                fprintf(new, "%d:", hrs);
            if (min < 10)
                fprintf(new, "0%d:", min);
            else
                fprintf(new, "%d:", min);
            sec+=0.0001;
            if (sec < 10)
            {
                holder[0] = '0';
                gcvt(sec, 5, &holder[1]);
                holder[2] = ',';
                holder[6] = '\0';
                fprintf(new, "%s", holder);
            }
            else
            {
                gcvt(sec, 6, &holder[0]);
                holder[2] = ',';
                holder[6] = '\0';
                fprintf(new, "%s", holder);
            }
            if(i==0)
                fprintf(new," --> ");
            else
                fprintf(new,"\n");
        }
        //copying the subtitle text
        fgets(lineStr, 200, old);
        fprintf(new, "%s", lineStr);
        //printf("%s",lineStr);
        fgets(lineStr, 200, old);
        fprintf(new, "%s", lineStr);
    }
    fclose(old);
    fclose(new);
    return 0;
}