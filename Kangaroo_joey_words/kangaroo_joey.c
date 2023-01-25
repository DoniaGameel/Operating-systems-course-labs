#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "stdbool.h"
#include "ctype.h"
int main(int argc, char **argv) 
{
	if (argc<3) 
    {	 
		printf("Too few arguments. Exiting!\n");
		exit(1);
	}
    char* LongString;
    char* ShortString; 
    bool AreEqual = false;

    if(strlen(argv[1])==strlen(argv[2]))
    {   bool b = true;
        for(int i = 0 ; i< strlen(argv[1]) ;i++)
        {
            if((tolower(argv[1][i]))!=(tolower(argv[2][i])))
            {
                b=false;
                break;
            }
                
        }
        if(b==false)
        {
            LongString=argv[1];
            ShortString=argv[2];
        }
        else
        {
            AreEqual=true;
        }
    }
    else if(strlen(argv[1])>strlen(argv[2]))
    {
        LongString=argv[1];
        ShortString=argv[2];
    }
    else
    {
        LongString=argv[2];
        ShortString=argv[1];
    }
    if(!AreEqual)
    {
        int j =0;
        int k =0;
        for(int i =0 ; i<strlen(ShortString);i++)
        {
            while(j<strlen(LongString)&&(tolower(LongString[j]))!=(tolower(ShortString[i])))
            {
                j++;
            }
            if((tolower(LongString[j]))==(tolower(ShortString[i])))
                k++;
            if(j==strlen(LongString)-1)
                break;
        }
        if(k==strlen(ShortString))
            printf("1\n");
        else
            printf("0\n");
    }
    else
        printf("0\n");
    
	return 0; 
}