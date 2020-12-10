#ifndef ANIMATION_HPP
#define ANIMATION_HPP

struct AnimationFrame
{
    float duration;
    SDL_Rect clip;
};

struct Animation
{
    std::vector<AnimationFrame> frames;
    bool looped;
};

struct AnimationState
{
    std::string anim;
    float timer;
    int frame;
};

// SHOULD NOT BE USED DIRECTLY -- USE THE ASSET MANAGER INSTEAD!!!
INTERNAL void LoadAnimation (Animation& animation, std::string file_name);
INTERNAL void FreeAnimation (Animation& animation);

//
// INTERFACE
//

INTERNAL void CreateAnimation (AnimationState& state, std::string animation_name);
INTERNAL void UpdateAnimation (AnimationState& state, float dt);
INTERNAL void ResetAnimation  (AnimationState& state);
INTERNAL bool IsAnimationDone (AnimationState& state);

INTERNAL const SDL_Rect* GetAnimationClip (AnimationState& state);

INTERNAL int GetAnimationFrameWidth  (AnimationState& state);
INTERNAL int GetAnimationFrameHeight (AnimationState& state);

#endif /* ANIMATION_HPP */
