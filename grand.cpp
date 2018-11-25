int save_h_s_iff(int score, int mode)       //save high score if and only if complex condition satisfy
{
    h_s_d h;
    time_t the_time = time(NULL);
    FILE *fptr = fopen(H_S_LOCATION,"rb+");
    if(c_fptr(fptr))   return 0;
    h.score = score;
    strcpy(h.date_t,ctime(&the_time));

    if(mode == 0)
    {
        if(fwrite(&h,sizeof(hh),1,fptr) < 1)
        {
        perror("Could not save high score " H_S_LOCATION);
        fclose(fptr);
        return 0;
        }
        fclose(fptr);
        return 1;       //this function returns 1 if successfull
    }
    if(mode == 1)
    {
        int i;
        h_s_d h1;
        D_h_s_sort(1);  //arrage high score in ascending order to evalutate such that first score gets compared to lower high score first
        rewind(fptr);                   //go to begining of file
        for(i=0;fread(&h1,sizeof(h1),1,fptr)==1;i++)
        {
            if(h1.score < h.score)
            {
                fseek(fptr, -(sizeof(h1)), SEEK_CUR);      //go 1 data before to rewrite/replace
                fwrite(&h, sizeof(h1), 1, fptr);
                fclose(fptr);
                return 1;      //is high score
            }
        }
    }


    fclose(fptr);
    return 0;
}
