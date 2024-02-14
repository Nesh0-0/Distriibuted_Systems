#include <stdio.h>
#include<pthread.h>

const int readers_size=2, writers_size=2;

int mutex=1, reader=readers_size, write=1; 
 
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
    static int readcount = 0;  
    wait (mutex);  
    readcount ++; // on each entry of reader increment readcount  
    if (readcount == 1)  
    {  
        wait (write);  
    }  
    signal(mutex);
    
    // wait(reader);  
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
    // signal(reader);
    
    wait(mutex);

    readcount --; // on every exit of reader decrement readcount  
    if (readcount == 0)  
    {  
        signal (write);  
    }  
    signal(mutex); 
    return NULL;
}

    
        


void *writer()
{
    wait(write);
    wait(mutex);
  
    
    FILE *fp = fopen("sample.txt", "w");  

    
    if (fp == NULL) {
        printf("Error opening file!\n");
        return NULL;
    }

    
    char text[] = "This text will be written to the file.\n";

    
    printf("%s", text);  

    
    fclose(fp);

    printf("Data written to file successfully!\n"); 
        

    signal(write);
    signal(mutex);

    return NULL;
}

void main()
{
    // read();
    // writer();
    pthread_t  readers[2];
    pthread_t  writers[2];

    for (int i=0; i<readers_size; i++)
    {
        pthread_create(&readers[i], NULL, read, NULL);
    }
    
    for (int i=0; i<writers_size; i++)
    {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i=0; i<readers_size; i++)
    {
        pthread_join(readers[i], NULL);
    }

    for (int i=0; i<writers_size; i++)
    {
        pthread_join(writers[i], NULL);
    }


}