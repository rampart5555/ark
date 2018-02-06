#ifndef __BASIC_SCENE__
#define __BASIC_SCENE__

#define SCENE 1
#define ENTITY_MANAGER 2

class BasicScene
{
    public:
        BasicScene() { m_sceneId = -1; }
        void setSceneId(int sid) { m_sceneId=sid; }
        int getSceneId() { return m_sceneId; }
    protected:
        int m_sceneId;
};
#endif