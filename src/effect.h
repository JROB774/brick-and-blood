#ifndef EFFECT_H
#define EFFECT_H

typedef enum EffectID__
{
    // Nothing...
    EFX_TOTAL

} EffectID;

typedef struct Effect__
{
    EffectID type;
    int palette;
    int frame;
    float x,y,vx,vy;
    bool alive;
    bool invis;
    float t;

} Effect;

#define EFFECT_MAX 4096

GLOBAL Effect gEffect[EFFECT_MAX];

INTERNAL void create_effect    (EffectID id, int x, int y, int w, int h, int min_count, int max_count);
INTERNAL void update_effect    (float dt);
INTERNAL void render_effect_lo (float dt); // Drawn before entities.
INTERNAL void render_effect_hi (float dt); // Drawn after entities.

#endif /* EFFECT_H */
