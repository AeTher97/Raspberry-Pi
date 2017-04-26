#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include"kbhit.h"

int check(char * napis)
{
    int i;
    int wynik=0;

    i=0;
    while(napis[i]!='\0')
    {
        if(napis[i]=='Y' || napis[i]=='E' || napis[i]=='S')
            wynik ++;
            i++;
    }

    if(wynik==3)
        return 0;
    else
        return -1;
}



int cyfra(char c)
{
    if(c==48)
        return 0;
    if(c==49)
        return 1;
    if(c==50)
        return 2;
    if(c==51)
        return 3;
    if(c==52)
        return 4;
    if(c==53)
        return 5;
    if(c==54)
        return 6;
    if(c==55)
        return 7;
    if(c==56)
        return 8;
    if(c==57)
        return 9;


}


float znajdz_numer(char * napis)
{
    int i=0;
    int znak=1;
    float numer=0;
    int dzielenie=0;

    while(napis[i]!='=')
        i++;

    i++;
    if(napis[i]=='-')
    {
        i++;
        znak = -1;
    }
    if(napis[i+4]<48||napis[i+4]>57)
        dzielenie=1;
    if(napis[i+3]<48||napis[i+3]>57)
        dzielenie=10;
    if(napis[i+2]<48||napis[i+2]>57)
        dzielenie=100;
    if(napis[i+1]<48||napis[i+1]>57)
        dzielenie=1000;

    numer=znak*(10*cyfra(napis[i])+cyfra(napis[i+1])+0.1*cyfra(napis[i+2])+0.01*cyfra(napis[i+3])+0.001*cyfra(napis[i+4]));
    if(dzielenie!=0)
        numer = numer/(dzielenie*10);

    return numer;

}

float data_capture()
{
    FILE *fp;
    char napis[100];
    float temperatura;

    system("cd /sys/bus/w1/devices/28-041686d00fff ; cat w1_slave > /home/pi/Desktop/temperatura.txt");
    fp=fopen("/home/pi/Desktop/temperatura.txt","r");
    fgets(napis,100,(FILE*)fp);


    if(check(napis)==0)
        fgets(napis,100,(FILE*)fp);
    else
        return -1000;

    fclose(fp);

    temperatura=znajdz_numer(napis);

    return temperatura;
}


void data_save(float temperatura, char * time)
{
    FILE *fp;
    fp=fopen("/home/pi/Desktop/temperatury.txt","a");
    fprintf(fp,"%s : %f\n",time,temperatura);
    fclose(fp);

}

char * delete_line_end(char * napis)
{
    int i=0;
    while(napis[i]!='\n')
        i++;
    napis[i]='\0';
    return napis;
}


char * current_time()
{

time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  //printf ( "Current local time and date: %s", asctime (timeinfo) );

  return asctime (timeinfo);
}


int main()
{
    int counter=0;
    char c='a';
    float temperatura;
    FILE *fp;

    while(c!='K' && c!='k')
    {

        temperatura=data_capture();

        system("clear");
        printf("%s : %f\n",delete_line_end(current_time()),temperatura);

        if(counter==30)
        {

            data_save(data_capture(),delete_line_end(current_time()));
            counter = 0;
        }

        if(kbhit())
            c = readch();
        if(temperatura!=-1000){
            fp=fopen("/var/www/html/temp.txt","w");
            fprintf(fp,"%f\n",temperatura);
            fclose(fp);
        }

        usleep(1000000);
        counter++;

    }


    return 0;
}

