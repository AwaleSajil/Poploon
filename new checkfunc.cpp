int check_w_b(s **sp, inf *inf1)
{
    int i;
    static int prev_i;
    POINT m_po;
    GetCursorPos(&m_po);
    for(i=(inf1->saver1);i<(inf1->indi);i++)
    {
        int t = sp[i]->asize;      //max distance
        int d = sqrt(pow((xcen+sp[i]->c) - (m_po.x),2) + pow((ycen+sp[i]->cy) - (m_po.y-15),2));    //distance between pointer and bubble center
        sp[i]->cy = rnd(-TARGET_HEIGHT/5,TARGET_HEIGHT/5); //randomize y cordinates
            if(d<=t && d>=(-t))
            {
                if(prev_i != i)
                    inf1->ssh = 0;      //reset the stop score high(ssh) if new bullune is on the target
                if(sp[i]->color != 15)
                {
                    return (prev_i=i);
                }

                if(sp[i]->color == 15 && inf1->prev_click == 1)
                    return (prev_i=i);
            }
        }

    return -1;          // i.e no bullunes lie at that position
}
