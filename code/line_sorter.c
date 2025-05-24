#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LENGHT_L 256
#define INITIAL_CAP 16
int compare_strings (const void *ptr_a,const void *ptr_b)
{
    const char *string_a=*(const char **)ptr_a;
    const char *string_b=*(const char **)ptr_b;
    return strcmp(string_a,string_b);
}
int main(int argc,char *argv[]){
    if (argc!=4){
        fprintf(stderr,"Error,incorrect value of arguments!\n");
        exit(EXIT_FAILURE);
    }

    printf("input file: '%s', output file: '%s',method '%s'\n",argv[1],argv[2],argv[3]);

    printf("opening of input file\n");
    FILE *input_stream = fopen(argv[1],"r");
    if (input_stream == NULL){
        perror("input file");
        EXIT_FAILURE;
    }
    printf("ready for reading!\n");
    
    char **lines_arr=NULL;
    size_t num_lines_stored=0;
    size_t current_capacity=0;

    char line_read_buffer[MAX_LENGHT_L];
    printf("reading...\n");
    while(fgets(line_read_buffer,MAX_LENGHT_L,input_stream)!= NULL){
        line_read_buffer[strcspn(line_read_buffer,"\n")]='\0';
        if (strlen(line_read_buffer)==0){
            continue;
        }
        if (num_lines_stored>=current_capacity){
            size_t new_capacity = (current_capacity==0) ? INITIAL_CAP : current_capacity * 2;
            char **temp_ptr=realloc(lines_arr,new_capacity * sizeof(char*));

            if (temp_ptr==NULL){
                perror("Error, memory (realloc)");
                for (size_t i=0;i<num_lines_stored;++i)
                {
                    free(lines_arr[i]);
                }
                free(lines_arr);
                fclose(input_stream);
                EXIT_FAILURE;
            }
            lines_arr=temp_ptr;
            current_capacity = new_capacity;
        }
        lines_arr[num_lines_stored]=strdup(line_read_buffer);
        if(lines_arr[num_lines_stored]==NULL){
            perror("Error, memory (duplicate)");
            for (size_t i=0;i<num_lines_stored;++i)
                {
                    free(lines_arr[i]);
                }
                free(lines_arr);
                fclose(input_stream);
                EXIT_FAILURE;
        }
        num_lines_stored++;
    }
    if (ferror(input_stream)){
        perror("Error,reading");
        for (size_t i=0;i<num_lines_stored;++i)
                {
                    free(lines_arr[i]);
                }
                free(lines_arr);
                fclose(input_stream);
                EXIT_FAILURE;
    }
    printf("reading is complete! %d \n",num_lines_stored);
    if (num_lines_stored>1){
        printf("sort %d lines by %s",num_lines_stored,argv[3]);
        qsort(lines_arr,num_lines_stored, sizeof(char*),compare_strings);
        printf("complete!\n");
    } else printf("1 line\n");

    FILE *output_stream=fopen(argv[2],"w");
    if(output_stream==NULL)
    {
        for (size_t i=0;i<num_lines_stored;++i)
                {
                    free(lines_arr[i]);
                }
                free(lines_arr);
                fclose(input_stream);
                EXIT_FAILURE;
    }
    printf("file is opend for writting\n");
    printf("writting sorted lines\n");

    for (size_t i=0;i<num_lines_stored;++i){
        printf("%s\n",lines_arr[i]);
        if (fputs(lines_arr[i],output_stream)==EOF){
            perror("Error, writting");
            fclose(output_stream);
            for (size_t j=0;j<num_lines_stored;++j) {
                free(lines_arr[j]);
            }
            free (lines_arr);
            EXIT_FAILURE;
        }
        if(fputc('\n',output_stream)==EOF){
            perror("error EOF");
            fclose(output_stream);
            for (size_t j=0;j<num_lines_stored;++j) {
                free(lines_arr[j]);
            }
            free (lines_arr);
            EXIT_FAILURE;
        }
    }
        if(fclose(output_stream)==EOF){
            perror("Error,closing");
            for (size_t j=0;j<num_lines_stored;++j) {
                free(lines_arr[j]);
            }
            free (lines_arr);
            EXIT_FAILURE;
    
        
        }
        for (size_t j=0;j<num_lines_stored;++j) {
             free(lines_arr[j]);
            lines_arr[j]=NULL;
        }
        free (lines_arr);
        lines_arr=NULL;
        printf("memory is cleared");
        return 0;
}