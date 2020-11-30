INTERNAL void create_effect (EffectID id, int x, int y, int w, int h, int min_count, int max_count)
{
    int count = random_int_range(min_count, max_count);

    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (!effect->alive)
        {
            effect->alive = true;
            effect->type = id;
            effect->invis = false;

            int fw = 0;
            int fh = 0;

            switch (effect->type)
            {
                // Nothing...
            }

            effect->x  = random_int_range(x,x+w) - (fw/2);
            effect->y  = random_int_range(y,y+h) - (fh/2);
            effect->vx = 0;
            effect->vy = 0;
            effect->t  = 0.0f;

            // Specific creation logic for the different effect types.
            switch (effect->type)
            {
                // Nothing...
            }

            count--;
            if (count <= 0)
            {
                break;
            }
        }
    }
}

INTERNAL void update_effect (float dt)
{
    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            // Specific update logic for the different effect types.
            switch (effect->type)
            {
                // Nothing...
            }
        }
    }
}

INTERNAL int effect_z_compare (const void* a, const void* b)
{
    const Effect* ea = CAST(Effect*, a);
    const Effect* eb = CAST(Effect*, b);

    if (ea->type < eb->type) return -1;
    if (ea->type > eb->type) return  1;

    return 0;
}

INTERNAL void render_effect_lo (float dt)
{
    // Sort the effect list so they render in the correct order. The order
    // is determiend by the order of the IDs in the EffectID enumeration.
    qsort(gEffect, EFFECT_MAX, sizeof(Effect), effect_z_compare);

    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            /*
            // We only want to render certain effect types on the low pass.
            if (effect->type != EFX_SCORE10  &&
                effect->type != EFX_SCORE20  &&
                effect->type != EFX_SCORE40  &&
                effect->type != EFX_SCORE80  &&
                effect->type != EFX_ICO_LIFE &&
                effect->type != EFX_ICO_TIME &&
                effect->type != EFX_ICO_MULT &&
                effect->type != EFX_ICO_RAPD &&
                effect->type != EFX_ICO_SPRD &&
                effect->type != EFX_ICO_BOOM)
            {
                // Specific render logic for the different effect types.
                const Clip* clip = NULL;;
                switch (effect->type)
                {
                    case (EFX_BLOOD    ): clip = ANM_BLOOD [effect->frame]; break;
                    case (EFX_GIB_BLOOD): clip = ANM_BLOOD [effect->frame]; break;
                    case (EFX_GIB      ): clip = ANM_BLOOD [effect->frame]; break;
                    case (EFX_CHIP_0   ): clip = ANM_CHIP  [effect->frame]; break;
                    case (EFX_CHIP_1   ): clip = ANM_CHIP  [effect->frame]; break;
                    case (EFX_BUBBLE   ): clip = ANM_BUBBLE[effect->frame]; break;
                    case (EFX_SHOT     ): clip = ANM_SHOT  [effect->frame]; break;
                }

                if (!effect->invis)
                {
                    render_bitmap(effect->x,effect->y, effect->palette, clip);
                }
            }
            */
        }
    }
}

INTERNAL void render_effect_hi (float dt)
{
    for (int i=0; i<EFFECT_MAX; ++i)
    {
        Effect* effect = gEffect+i;
        if (effect->alive)
        {
            /*
            // We only want to render certain effect types on the high pass.
            if (effect->type == EFX_SCORE10  ||
                effect->type == EFX_SCORE20  ||
                effect->type == EFX_SCORE40  ||
                effect->type == EFX_SCORE80  ||
                effect->type == EFX_ICO_LIFE ||
                effect->type == EFX_ICO_TIME ||
                effect->type == EFX_ICO_MULT ||
                effect->type == EFX_ICO_RAPD ||
                effect->type == EFX_ICO_SPRD ||
                effect->type == EFX_ICO_BOOM)
            {
                // Specific render logic for the different effect types.
                const Clip* clip = NULL;
                switch (effect->type)
                {
                    case (EFX_SCORE10  ): clip = &SPR_SCORE10;    break;
                    case (EFX_SCORE20  ): clip = &SPR_SCORE20;    break;
                    case (EFX_SCORE40  ): clip = &SPR_SCORE40;    break;
                    case (EFX_SCORE80  ): clip = &SPR_SCORE80;    break;
                    case (EFX_ICO_LIFE ): clip = &SPR_ICO_LIFE_0; break;
                    case (EFX_ICO_TIME ): clip = &SPR_ICO_TIME_0; break;
                    case (EFX_ICO_MULT ): clip = &SPR_ICO_MULT_0; break;
                    case (EFX_ICO_RAPD ): clip = &SPR_ICO_RAPD_0; break;
                    case (EFX_ICO_SPRD ): clip = &SPR_ICO_SPRD_0; break;
                    case (EFX_ICO_BOOM ): clip = &SPR_ICO_BOOM_0; break;
                }

                if (!effect->invis)
                {
                    render_bitmap(effect->x,effect->y, effect->palette, clip);
                }
            }
            */
        }
    }
}
