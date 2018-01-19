#include "Scene.h"

static char* readBinaryFile(const char* filename, unsigned int *filesize)
{
    unsigned int bufsize=0;
    char *buf = NULL;
        
    FILE *fp = fopen (filename, "rb");

    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        bufsize = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buf = (char*)malloc (bufsize+1);
        fread (buf, 1, bufsize, fp);
        fclose (fp);
        buf[bufsize] = 0;
    }
    *filesize = bufsize;
    
    return buf;
}

Scene::Scene()
{
    m_rootNode = new osg::PositionAttitudeTransform();
    printf("Scene::m_rootNode:%p\n",m_rootNode);
}

Scene::~Scene()
{
    printf("Scene Destructor\n");
}

void Scene::handleSceneEvent()
{
}

osg::ref_ptr<osg::Program> Scene::createProgram()
{
    return NULL;
}

void Scene::createScene()
{        

}

osg::PositionAttitudeTransform* Scene::getSceneNode()
{
    return m_rootNode.get();
}

osg::Program* Scene::loadProgram(const char *vs, const char *fs)
{
    unsigned int filesize;
    char *vert_shader = readBinaryFile(vs,&filesize);
    char *frag_shader = readBinaryFile(fs,&filesize);
    if((vert_shader==NULL)||(frag_shader==NULL))
    {
        printf("shader load failed\n");
        return NULL;
    }
    osg::Program *program = new osg::Program;
    program->setName( "Shader" );
    osg::Shader *vert_s = new osg::Shader( osg::Shader::VERTEX, vert_shader);
    osg::Shader *frag_s = new osg::Shader( osg::Shader::FRAGMENT, frag_shader );
    vert_s->setName(vs);
    frag_s->setName(fs);
    program->addShader( vert_s );
    program->addShader( frag_s );
    return program;
    
}

osg::Texture2D* Scene::loadTexture(const char *img_name)
{
     osg::Image *image = osgDB::readImageFile(img_name);
     osg::Texture2D *texture = new osg::Texture2D;
     texture->setDataVariance(osg::Object::DYNAMIC);
     texture->setImage(image);
     texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
     texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
     texture->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
     texture->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
     return texture;
}

osg::Node* Scene::loadSceneModel(const char *model_name)
{
    osg::Node *node = osgDB::readNodeFile(model_name);
    return node;    
}









