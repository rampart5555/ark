#ifndef __CALLBACKS__
#define __CALLBACKS__
typedef bool (*EngineCallback)(void *args);
bool show_statistics(void *args);
bool play_animation(void *args);
#endif
