
locations = {
    door_right = { x=1.2,  y=-1.14,  z=0.0},
    door_right_open_pin =  { x=1.2,  y=-1.14 + 0.25,  z=0.0},
    door_right_close_pin =  { x=1.2,  y=-1.14 - 0.25,  z=0.0},    
    door_left = { x=-1.2, y=-1.14, z=0.0 },
    paddle = { x=0.0, y=0.0, z=0.0 },
    paddle_slot_1 = { x=-0.5, y=1.0, z=0.0 },
    paddle_slot_2 = { x=-0.5, y=1.0, z=0.0 },
    paddle_slot_3 = { x=-0.5, y=1.0, z=0.0 },
    paddle_slot_4 = { x=-0.5, y=1.0, z=0.0 },
    paddle_slot_5 = { x=-0.5, y=1.0, z=0.0 }
}

Animations = {
    {
        name="door_left_open",
        frames={ 
            0.0417, -1.2000, -1.1402, -0.0000,
            0.2917, -1.2000, -1.0968, -0.0000,
            0.5417, -1.2000, -0.9859, -0.0000,
            0.7917, -1.2000, -0.8823, -0.0000,
            1.0417, -1.2000, -0.8521, -0.0000
        }
    },    
    {
        name="door_right_open",
        frames={ 
            0.0417, 1.2000, -1.1402, -0.0000,
            0.2917, 1.2000, -1.0968, -0.0000,
            0.5417, 1.2000, -0.9859, -0.0000,
            0.7917, 1.2000, -0.8823, -0.0000,
            1.0417, 1.2000, -0.8521, -0.0000
        }
    },    
    {
        name="paddle_new_1",
        frames={ 
            0.0, locations.paddle_slot_1.x, locations.paddle_slot_1.y, locations.paddle_slot_1.z,
            0.5, locations.paddle_slot_1.x, locations.paddle_slot_1.y, locations.paddle_slot_1.z + 1.0,
            1.0, locations.paddle.x, locations.paddle.y, locations.paddle.z + 1.0 ,
            1.5, locations.paddle.x, locations.paddle.y, locations.paddle.z - 1.0 
        }
    }
}

demo1={
    level_complete={
        {
            target="door_left_open",
            frames={0.0}
        },
        {
            target="paddle_move",
            frames={0.0}
        }
    }
}

function getAnimationTableSize()
    return #Animations
end

function getAnimation(anim_index)
    return Animations[anim_index]
end


