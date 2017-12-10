ROOT_PATH = "/home/dcioata/g_dev/ark/assets/"
Resources={
    { rtype="osgt", rid="scene_models",  
        file1_path=ROOT_PATH .. "models/scene_models.osgt",   file2_path="" },
    { rtype="osgt", rid="widget_models", 
        file1_path=ROOT_PATH .. "models/widget_models.osgt",  file2_path=""},
    { rtype="glsl", rid="model_texture", 
        file1_path=ROOT_PATH .. "shaders/model_texture.vert", file2_path=ROOT_PATH .. "shaders/model_texture.frag" },
    { rtype="glsl", rid="model_color",   
        file1_path=ROOT_PATH .. "shaders/model_color.vert",   file2_path=ROOT_PATH .. "shaders/model_color.frag" },
    { rtype="glsl", rid="widget_color",  
        file1_path=ROOT_PATH .. "shaders/widget_color.vert",  file2_path=ROOT_PATH .. "shaders/widget_color.frag"},
    { rtype="jpeg", rid="brick_read",    
        file1_path=ROOT_PATH .. "images/brick_red.jpeg",      file2_path="" },
    { rtype="ttf",  rid="icomoon", 
        file1_path=ROOT_PATH .. "fonts/IcoMoon-Free.ttf",     file2_path="" }
}
