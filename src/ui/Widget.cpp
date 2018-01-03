#include <stdio.h>
#include "Widget.h"
#include "MenuManager.h"
#include "scene/LevelManager.h"
#include "Config.h"
#include "Logging.h"

static unsigned int widget_id=0;

Widget::Widget()
{
    m_width = 0;
    m_height = 0;    
    m_label = NULL;
    m_geomNode = NULL;
    m_textGeode = NULL;
    m_callback  = NULL;
    m_transform = NULL;

    m_widgetId = widget_id++;
}

Widget::Widget(MenuItem item)
{
    osg::MatrixTransform *node = AssetsManager::instance().getWidgetModel(item.m_model.c_str());
    if(node==NULL)
    {
        LOG_ERROR("widget model not found:%s\n",item.m_model.c_str()); 
        return;
    }
    setNode(node);
    setLabel(item.m_label.c_str());
    setPosition(item.m_x, item.m_y, 0.0f);
    setScale(item.m_width,item.m_height, 1.0f);
    setCallback( getCallback(item.m_cb) );
    m_width = item.m_width;
    m_height = item.m_height;
    m_widgetId = widget_id++;
    //printf("name: %s position: %f, %f\n", item.m_name.c_str(), item.m_x, item.m_y);
    
}

unsigned int Widget::getId()
{
    return m_widgetId;
}
void Widget::setNode(osg::MatrixTransform *node)
{
    if(node == NULL)
    {        
        return;
    }
    m_transform = new osg::MatrixTransform();
    m_geomNode = new osg::MatrixTransform(*node, osg::CopyOp::DEEP_COPY_ALL);
    m_geomNode->setMatrix(osg::Matrix::translate(0.0f, 0.0f, -0.3f));
    osg::Geode *geode = dynamic_cast <osg::Geode*>(m_geomNode->getChild(0));
    osg::Drawable *dr = geode->getDrawable(0);
    dr->setUserData(this);        
    m_transform->addChild(m_geomNode);
    
    /* Label */
    m_label = new osgText::Text;
    m_label->setUserData(this);
    m_label->setFont(AssetsManager::instance().getFont("icomoon"));
    m_label->setCharacterSize(0.5f);
    m_label->setFontResolution(48,48);
    m_label->setDrawMode(osgText::Text::TEXT);    
    m_label->setAlignment(osgText::Text::CENTER_CENTER);    
    m_label->setPosition(osg::Vec3(0.0f, 0.0f, -0.2f));
    osg::Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    m_label->setColor(color);
    m_textGeode = new osg::Geode();
    m_textGeode->addDrawable(m_label);
    m_transform->addChild(m_textGeode);
    
    /* Shader */
    osg::Program *program = AssetsManager::instance().getProgram("widget_color");        
    osg::StateSet *ss = m_transform->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    //ss->addUniform(uniform);    
    ss->setAttribute(program); 

    m_geomNode->getOrCreateStateSet()->setDefine("WIDGET_GEOM");
    m_textGeode->getOrCreateStateSet()->setDefine("WIDGET_TEXT");
    
}

void Widget::setCallback(EngineCallback cb)
{
    m_callback=cb;
}

void Widget::runCallback(void *args)
{
    if(m_callback != NULL)
        m_callback(args);
}

osg::MatrixTransform* Widget::getNode()
{
    return m_transform;
}

void Widget::setPosition(float x,float y, float z)
{
    if(m_transform==NULL) return;
    osg::Matrix m;
    m.makeTranslate(x,y,z);
    
    m_transform->setMatrix(m);   
}

void Widget::setScale(float x, float y, float z)
{
    if(m_geomNode==NULL) 
        return;
    /* Actualy make translate and scale to avoid ovelapping text */
    m_geomNode->setMatrix(osg::Matrix::translate(0.0f, 0.0f, -0.3f)*osg::Matrix::scale(x,y,z));
}

void Widget::setLabel(const char* text)
{
    if(m_label==NULL) return;
    
    m_label->setText(text, osgText::String::ENCODING_UTF8);
}

float Widget::getWidth()
{
    return m_width;
}

float Widget::getHeight()
{
    return m_height;
}

void Widget::setUniform(osg::Uniform *uniform)
{
#if 1
    if(m_transform==NULL) return;
    osg::StateSet *ss = m_transform->getOrCreateStateSet();    
    ss->addUniform(uniform);    
#endif    
}

/* level selected button */
LevelSelectButton::LevelSelectButton():Widget()
{
    m_epId = -1;   
    m_lvlId = -1;   
}

void LevelSelectButton::setSceneData(int ep_id, int lvl_id)
{    
    m_epId = ep_id;
    m_lvlId = lvl_id;
    
    SceneLevel *lvl = LevelManager::instance().getLevel(m_epId, m_lvlId);
    if( lvl == NULL)
    {
        LOG_ERROR("Level not found for episode:%d level:%d\n", m_epId, m_lvlId);
        return;
    }    
    if(lvl->m_unlocked == false)    
        setLabel("\uE98F");
    else
    {
        char buf[10];
        sprintf(buf, "%d", lvl->m_id+1);
        setLabel(buf);        
    }
}

void LevelSelectButton::runCallback(void *args)
{
    SceneLevel *lvl;    
    lvl = LevelManager::instance().getLevel(m_epId, m_lvlId);
    if(lvl == NULL)
    {
        LOG_ERROR("Level not found for episode:%d level:%d\n", m_epId, m_lvlId);
        return;
    }
    //LOG_INFO("Level data: ep_id: %d lvl_id: %d  map: %s\n", m_epId, m_lvlId, li.lvl_data.c_str());

    if(m_callback != NULL)
        m_callback(lvl);    
}


