
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
        name="Restart", 
        model="widget_square",  
        cb="MenuLevelFailed_button_continue_push", 
        label="Continue",
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
        x=0.0, y=5.0, width=4.0, height=0.75  
    },    
    { 
        name="Resume",
        model="widget_square", 
        cb="Scene_resume", 
        label="Resume", 
        x=2.0, y=-5.0, width=1.0, height=1.0  
    },        
    { 
        name="Pause",
        model="widget_square", 
        cb="Scene_pause", 
        label="Pause", 
        x=-2.0, y=-5.0, width=1.0, height=1.0  
    },
    { 
        name="HudScore",
        model="widget_square", 
        cb="", 
        label="0", 
        x=3.0, y=5.0, width=1.0, height=0.75  
    }
    
}
