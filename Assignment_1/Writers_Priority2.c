#include <stdio.h>
#include<pthread.h>

const int readers_size=3, writers_size=3;

int mutex=1, reader=1, write=1; 
static int writecount = 0;

 
void wait( int S )  
{  
    while( S <= 0) ;  
    S--;  
}  
      
void signal( int S )  
{  
    S++;  
}


void *read()
{
    while(writecount>0);    

    static int readcount = 0;
    wait(write);
    wait(mutex);
    readcount++;
    if (readcount == 1)
    {
        wait(write);
    } 
    signal(mutex); 
    FILE *fp = fopen("sample.txt", "r");

    if (fp == NULL) 
    {
        printf("Error opening file!\n");
        return NULL;
    }
    char word[100];
    printf("Reading the file\n");  
    while (fgets(word, sizeof(word), fp) != NULL) 
    {
        printf("%s", word);
    }
    fclose(fp);
    printf("Done reading!\n");
    wait(mutex);
    readcount--;
    if (readcount == 0) 
    {
        signal(write);
    }
    signal(mutex);
    signal(write);
   
    return NULL;
}


void *writer()
{
   
    wait(mutex);
    writecount++;
    printf("Number of writers are %d\n", writecount);
    signal(mutex);
   


    wait(write);  
    
    
    FILE *fp = fopen("sample.txt", "w");  

    
    if (fp == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }

    
    char text[] = "This text will be written to the file.\n";

    
    printf("%s", text);
    fprintf(fp, "%s", text);  

    
    fclose(fp);

    printf("Data written to file successfully!\n"); 
   
    signal(write);
     wait(mutex);
    writecount--;
    signal(mutex);
    
   
    
    return NULL;
}

void main()
{
    // read();
    // writer();
    pthread_t  readers[readers_size];
    pthread_t  writers[writers_size];

    for (int i=0; i<writers_size; i++)
    {
        pthread_create(&writers[i], NULL, writer, NULL);
    }
    for (int i=0; i<readers_size; i++)
    {
        pthread_create(&readers[i], NULL, read, NULL);
    }
    

    for (int i=0; i<writers_size; i++)
    {
        pthread_join(writers[i], NULL);
    }
    for (int i=0; i<readers_size; i++)
    {
        pthread_join(readers[i], NULL);
    }



}