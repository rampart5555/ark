
MenuLevelComplete=
{
    { 
        name="Title",
        model="widget_square", 
        cb="", 
        label="Level Complete", 
        x=0.0, y=2.3, width=4.0, height=0.75  
    },
    {   
        name="Score", 
        model="widget_square", 
        cb="", 
        label="1234",  
        x=0.0, y=0.8, width=3.0, height=0.75   
    },
    { 
        name="Levels",  
        model="widget_square",  
        cb="MenuLevelComplete_button_levels_push", 
        label="Levels",
        x = -1.75, y = -1.3, width=1.0, height=1.0 
    },
    { 
        name="Restart", 
        model="widget_square",  
        cb="MenuLevelComplete_button_restart_push", 
        label="Restart",
        x=0.0, y=-1.3, width=1.0, height=1.0
    },
    { 
        name="Next", 
        model="widget_square",  
        cb="MenuLevelComplete_button_restart_push",
        label="Next",
        x=1.75,  y=-1.3, width=1.0, height=1.0
    }
}

MenuLevelFailed=
{
    { 
        name="Title",
        model="widget_square", 
        cb="", 
        label="Level Failed", 
        x=0.0, y=2.3, width=4.0, height=0.75  
    },
    {   
        name="Score", 
        model="widget_square", 
        cb="", 
        label="1234",  
        x=0.0, y=0.8, width=3.0, height=0.75   
    },
    { 
        name="Levels",  
        model="widget_square",  
        cb="MenuLevelFailed_button_levels_push", 
        label="Levels",
        x = -1.75, y = -1.3, width=1.0, height=1.0 
    },
    { 
        name="Restart", 
        model="widget_square",  
        cb="MenuLevelFailed_button_restart_push", 
        label="Restart",
        x = 1.75, y=-1.3, width=1.0, height=1.0
    },
}

MenuSceneHud = 
{
    { 
        name="Title",
        model="widget_square", 
        cb="", 
        label="Scene Hud", 
        x=0.0, y=4.0, width=4.0, height=0.75  
    },    
    { 
        name="Start",
        model="widget_square", 
        cb="Scene_start_physics", 
        label="Start", 
        x=1.75, y=-4.0, width=1.0, height=1.0  
    },
    { 
        name="Pause",
        model="widget_square", 
        cb="Scene_stop_physics", 
        label="Pause", 
        x=-1.75, y=-4.0, width=1.0, height=1.0  
    },
    { 
        name="HudScore",
        model="widget_square", 
        cb="", 
        label="0", 
        x=3.0, y=4.5, width=1.0, height=1.0  
    }
    
}
