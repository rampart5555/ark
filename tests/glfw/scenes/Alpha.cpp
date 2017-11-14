#include "Alpha.h"

class FindModel : public osg::NodeVisitor
{
    public:            
        FindModel(const char *model_name): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
            m_name=model_name;
            m_model=NULL;            
        } 
        void apply(osg::MatrixTransform& tr)
        {              
            if(tr.getName()==m_name)
            {
                m_model=new osg::MatrixTransform(tr);
            }
            //traverse(tr);
        }
        osg::MatrixTransform *m_model;
        std::string m_name;
};

Alpha::Alpha():Scene()
{
    printf("Simple::m_rootNode:%p\n", m_rootNode);
}

Alpha::~Alpha()
{
    printf("Simple Destructor\n");
}

void Alpha::addModel(const char* model_name, const char *tex_name, const char *vs_name, const char *fs_name)
{
    osg::MatrixTransform *model = NULL;
    osg::Node *node = loadSceneModel("models/scene_models.osgt");    
    FindModel fm(model_name);
    node->accept(fm);
    model = fm.m_model;
    osg::Texture2D *tex = loadTexture(tex_name);
    osg::StateSet* ss = model->getOrCreateStateSet();
    ss->setTextureAttributeAndModes(0,tex,osg::StateAttribute::ON);
    osg::Program *prog = loadProgram(vs_name, fs_name);
    ss->setAttributeAndModes(prog, osg::StateAttribute::ON);
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    m_rootNode->addChild(model);
    
}

void Alpha::createScene()
{   
         
    addModel("Icosphere", "images/texture_1.png","shaders/texture.vert","shaders/texture.frag");
    addModel("Plane", "images/texture_2.png","shaders/texture.vert","shaders/texture.frag");
}










