#include "TestRefptr.h"

osg::ref_ptr <osg::Node> static_node;
osg::ref_ptr <osg::Node> static_node_1;

TestRefptr::TestRefptr()
{

}

TestRefptr::~TestRefptr()
{
    
}

void TestRefptr::createScene()
{
    m_node=new osg::Node();
    static_node=m_node;
    static_node_1=m_node;
}

void TestRefptr::handleSceneEvent()
{  
    printf("TestRefptr::handleSceneEvent() \n");
    static_node=NULL;
    static_node_1=NULL;
    printf("m_node:%d\n",m_node.valid());
    printf("static_node_1:%d\n",static_node_1.valid());        
    m_node=NULL;
    printf("m_node:%d\n",m_node.valid());
}
