INTERNAL void LoadAnimation (Animation& animation, std::string file_name)
{
    GonObject gon = GonObject::Load(file_name);
    if (gon.type != GonObject::FieldType::OBJECT)
    {
        LOG_ERROR(ERR_MAX, "Animation not a GON object! (%s)", file_name.c_str());
        return;
    }

    animation.looped = gon["looped"].Bool();

    float x = (float)gon["start_position"][0].Number();
    float y = (float)gon["start_position"][1].Number();
    float w = (float)gon["frame_size"    ][0].Number();
    float h = (float)gon["frame_size"    ][1].Number();

    int frame_count = gon["frame_count"].Int();
    animation.frames.clear();
    for (int i=0; i<frame_count; ++i)
    {
        float duration = (float)gon["frame_durations"][i].Number();
        animation.frames.push_back({ duration, { x,y,w,h } });
        x += w;
    }
}

INTERNAL void FreeAnimation (Animation& animation)
{
    animation.frames.clear();
}

//
// INTERFACE
//

INTERNAL void CreateAnimation (AnimationState& state, std::string animation_name)
{
    state.anim = animation_name;
    state.timer = 0, state.frame = 0;
}

INTERNAL void UpdateAnimation (AnimationState& state, float dt)
{
    Animation* animation = GetAsset<AssetAnimation>(state.anim);
    if (!animation) return;

    if (animation->looped || state.frame < animation->frames.size())
    {
        float time = 0.0f;
        for (int i=0; i<animation->frames.size(); ++i)
        {
            time += animation->frames.at(i).duration;
            if (state.timer <= time)
            {
                state.frame = i;
                break;
            }
        }

        state.timer += dt;

        // Handles wrapping back round to the start of the animation if we're looped.
        if (animation->looped)
        {
            float total_time = 0.0f;
            for (auto& frame: animation->frames)
            {
                total_time += frame.duration;
            }

            if (total_time > 0.0f)
            {
                while (state.timer >= total_time)
                {
                    state.timer -= total_time;
                }
            }
        }
    }
}

INTERNAL void ResetAnimation (AnimationState& state)
{
    state.timer = 0;
    state.frame = 0;
}

INTERNAL bool IsAnimationDone (AnimationState& state)
{
    Animation* animation = GetAsset<AssetAnimation>(state.anim);
    if (!animation) return false;
    if (animation->looped) return false;
    float total_time = 0.0f;
    for (auto& frame: animation->frames) total_time += frame.duration;
    if (state.timer >= total_time) return true;
    return false;
}

INTERNAL const Rect* GetAnimationClip (AnimationState& state)
{
    Animation* animation = GetAsset<AssetAnimation>(state.anim);
    if (!animation) return NULL;
    return &animation->frames.at(state.frame).clip;
}

INTERNAL float GetAnimationFrameWidth (AnimationState& state)
{
    const Rect* clip = GetAnimationClip(state);
    return (clip) ? clip->w : 0;
}

INTERNAL float GetAnimationFrameHeight (AnimationState& state)
{
    const Rect* clip = GetAnimationClip(state);
    return (clip) ? clip->h : 0;
}
