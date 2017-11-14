
struct ps_template{
//ptemplate
    float lifetime;
    float radius;
    float mass;
    bool  emissive;
    float size_range[2];
    float alpha_range[2];
    float color_range[6];    
//counter
    int counter_rate_range[2];
//placer
    int placer_type;
    float placer_center[3];
    float placer_radius_range[2];
    float placer_phi_range[2];    
//shooter
    float shooter_speed_range[2];
    float shooter_theta_range[2];
};
