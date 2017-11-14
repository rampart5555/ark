#include <stdio.h>
#include "SceneData.h"

EpisodeData Episode[EPISODE_NR]={

    {1, true, "Episode 1",
        {
            { 1,  true,  0, "levels/level_01.lua"},
            { 2,  false, 0, "levels/level_02.lua"},
            { 3,  false, 0, "levels/level_03.lua"},
            { 4,  false, 0, "levels/level_04.lua"},
            { 5,  false, 0, "levels/level_05.lua"},
            { 6,  false, 0, "levels/level_06.lua"},
            { 8,  false, 0, "levels/level_07.lua"},
            { 9,  false, 0, "levels/level_08.lua"},
            { 9,  false, 0, "levels/level_09.lua"},
            { 10, false, 0, "levels/level_10.lua"},
            { 11, false, 0, "levels/level_11.lua"},
            { 12, false, 0, "levels/level_12.lua"},
            { 13, false, 0, "levels/level_13.lua"},
            { 14, false, 0, "levels/level_14.lua"},
            { 15, false, 0, "levels/level_15.lua"}
         }
    },
    {2, false, "Episode 2",
        {
            { 1,  false, 0, "levels/level_01.lua"},
            { 2,  false, 0, "levels/level_01.lua"},
            { 3,  false, 0, "levels/level_01.lua"},
            { 4,  false, 0, "levels/level_01.lua"},
            { 5,  false, 0, "levels/level_01.lua"},
            { 6,  false, 0, "levels/level_01.lua"},
            { 8,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 10, false, 0, "levels/level_01.lua"},
            { 11, false, 0, "levels/level_01.lua"},
            { 12, false, 0, "levels/level_01.lua"},
            { 13, false, 0, "levels/level_01.lua"},
            { 14, false, 0, "levels/level_01.lua"},
            { 15, false, 0, "levels/level_01.lua"}
        }
    },
    {3, false, "Episode 3",
        {
            { 1,  true,  0, "levels/level_01.lua"},
            { 2,  false, 0, "levels/level_01.lua"},
            { 3,  false, 0, "levels/level_01.lua"},
            { 4,  false, 0, "levels/level_01.lua"},
            { 5,  false, 0, "levels/level_01.lua"},
            { 6,  false, 0, "levels/level_01.lua"},
            { 8,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 9,  false, 0, "levels/level_01.lua"},
            { 10, false, 0, "levels/level_01.lua"},
            { 11, false, 0, "levels/level_01.lua"},
            { 12, false, 0, "levels/level_01.lua"},
            { 13, false, 0, "levels/level_01.lua"},
            { 14, false, 0, "levels/level_01.lua"},
            { 15, true , 0, "levels/level_01.lua"}
        }
    }
};

void episode_dump(int ep_id)
{
    if(ep_id >= EPISODE_NR)
    {
        return;
    }
    EpisodeData *ep = &Episode[ep_id];
    printf("id: %d unlocked: %d name: %s\n", ep->m_id, ep->m_unlocked, ep->m_name);
}
