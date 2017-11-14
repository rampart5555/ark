#include "Widget.h"
#include <stdio.h>
static const char *widget_vert = {
"#version 100\n"
"#pragma import_defines ( WIDGET_GEOM, WIDGET_TEXT)\n"
"varying vec4 vColor;\n"
"varying vec2 vTexCoord0;\n"
"void main(void)\n"
"{\n"	
"#if defined(WIDGET_GEOM)\n"
"	vColor = vec4(0.0, 0.0, 1.0, 1.0);\n"		
"#elif defined(WIDGET_TEXT)\n"
"	vColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"#else\n"
"    vColor = vec4(0.5, 0.5, 0.5, 1.0);\n"
"#endif\n"
"	vTexCoord0 = gl_MultiTexCoord0.xy;\n"
"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
"}\n"
};

static const char *widget_frag = {
"#version 100\n"
"precision mediump float;\n"
"varying vec4 vColor;\n"
"varying vec2 vTexCoord0;\n"
"uniform sampler2D GlyphTexture;\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = vColor * texture2D(GlyphTexture, vTexCoord0).aaaa;\n"
"}\n"

};


Widget::Widget()
{
    m_callback=NULL;
    m_node = new osg::MatrixTransform();
    m_geom = new osg::MatrixTransform();
    osg::Geode* geode = new osg::Geode();
    osg::Drawable *box = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0f,0.0f,-0.1f),1.0f,1.0f,0.1f));
    box->setUserData(this);
    geode->addDrawable(box);
    m_geom->addChild(geode);
    m_node->addChild(m_geom);    
    osg::StateSet *ss = m_node->getOrCreateStateSet();
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    ss->setAttribute(createProgram()); 
    m_geom->getOrCreateStateSet()->setDefine("WIDGET_GEOM");
    
}

osg::Program* Widget::createProgram()
{
    osg::Program *program = new osg::Program;
    program->setName( "shader" );
    osg::Shader *vert_s = new osg::Shader( osg::Shader::VERTEX, widget_vert);
    osg::Shader *frag_s = new osg::Shader( osg::Shader::FRAGMENT, widget_frag );
    vert_s->setName("widget_vertex_shader");
    frag_s->setName("widget_fragment_shader");
    program->addShader( vert_s );    
    program->addShader( frag_s );
    return program;    
}

void Widget::setLabel(const char *label)
{
    m_label = new osgText::Text;
    m_label->setUserData(this);
    //m_label->setFont(AssetsManager::instance().getFont("icomoon"));
    m_label->setCharacterSize(0.2);
    m_label->setFontResolution(48,48);
    m_label->setDrawMode(osgText::Text::TEXT);    
    m_label->setAlignment(osgText::Text::CENTER_CENTER);    
    m_label->setPosition(osg::Vec3(0.0f, 0.0f, 0.0f));
    osg::Vec4 color(1.0f, 1.0f, 1.0f, 1.0f);
    m_label->setColor(color);
    m_label->setText(label, osgText::String::ENCODING_UTF8);
    osg::Geode *geode = new osg::Geode();
    geode->addDrawable(m_label);
    geode->getOrCreateStateSet()->setDefine("WIDGET_TEXT");
    m_node->addChild(geode);
}

void Widget::setPosition(float x,float y,float z)
{
    osg::Matrix m;
    m.makeTranslate(x,y,z);    
    m_node->setMatrix(m);   
}

void Widget::setCallback(EngineCallback cb)
{
    m_callback=cb;
}

bool Widget::runCallback()
{
    bool ret_code = false;
    if(m_callback != NULL)
        ret_code = m_callback(NULL);
    else
        printf("Widget callback not found!\n");
    return ret_code;  
}
