// based on https://www.opengl.org/archives/resources/features/StencilTalk/tsld021.htm
#if 0
	var shadowMatrix = new THREE.Matrix4();

	return function ( plane, lightPos ) {
		
		var dot =  planeNormal.x * lightPos.x +
			   planeNormal.y * lightPos.y +
			   planeNormal.z * lightPos.z +
			  -plane.constant * lightPos.w;

		var sme = shadowMatrix.elements;

		sme[ 0 ]  = dot - lightPos.x *  planeNormal.x;
		sme[ 4 ]  = - lightPos.x *  planeNormal.y;
		sme[ 8 ]  = - lightPos.x *  planeNormal.z;
		sme[ 12 ] = - lightPos.x * -plane.constant;

		sme[ 1 ]  = - lightPos.y *  planeNormal.x;
		sme[ 5 ]  = dot - lightPos.y *  planeNormal.y;
		sme[ 9 ]  = - lightPos.y *  planeNormal.z;
		sme[ 13 ] = - lightPos.y * -plane.constant;

		sme[ 2 ]  = - lightPos.z *  planeNormal.x;
		sme[ 6 ]  = - lightPos.z *  planeNormal.y;
		sme[ 10 ] = dot - lightPos.z *  planeNormal.z;
		sme[ 14 ] = - lightPos.z * -plane.constant;

		sme[ 3 ]  = - lightPos.w *  planeNormal.x;
		sme[ 7 ]  = - lightPos.w *  planeNormal.y;
		sme[ 11 ] = - lightPos.w *  planeNormal.z;
		sme[ 15 ] = dot - lightPos.w * -plane.constant;
		
		this.matrix.multiplyMatrices( shadowMatrix, this.meshMatrix );
#endif		

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/ref_ptr>
#include <osg/Light>
#include <osg/LightSource>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osgGA/StateSetManipulator>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>

void shadowMatrix(float shadowMat[4][4], float groundplane[4], float lightpos[4]) 

{ 

    float dot; 
    int X=0,Y=1,Z=2,W=3;
    /* Find dot product between light position vector and ground plane normal. */ 

    dot = groundplane[X] * lightpos[X] + 
    groundplane[Y] * lightpos[Y] + 
    groundplane[Z] * lightpos[Z] + 
    groundplane[W] * lightpos[W]; 

    shadowMat[0][0] = dot - lightpos[X] * groundplane[X]; 
    shadowMat[1][0] = 0.f - lightpos[X] * groundplane[Y]; 
    shadowMat[2][0] = 0.f - lightpos[X] * groundplane[Z]; 
    shadowMat[3][0] = 0.f - lightpos[X] * groundplane[W]; 
    shadowMat[X][1] = 0.f - lightpos[Y] * groundplane[X]; 
    
    shadowMat[1][1] = dot - lightpos[Y] * groundplane[Y]; 
    shadowMat[2][1] = 0.f - lightpos[Y] * groundplane[Z]; 
    shadowMat[3][1] = 0.f - lightpos[Y] * groundplane[W]; 
    shadowMat[X][2] = 0.f - lightpos[Z] * groundplane[X]; 
    shadowMat[1][2] = 0.f - lightpos[Z] * groundplane[Y]; 
    
    shadowMat[2][2] = dot - lightpos[Z] * groundplane[Z]; 
    shadowMat[3][2] = 0.f - lightpos[Z] * groundplane[W]; 
    shadowMat[X][3] = 0.f - lightpos[W] * groundplane[X]; 
    shadowMat[1][3] = 0.f - lightpos[W] * groundplane[Y]; 
    shadowMat[2][3] = 0.f - lightpos[W] * groundplane[Z]; 
    shadowMat[3][3] = dot - lightpos[W] * groundplane[W]; 

}

void setShader(osg::MatrixTransform *model)
{
    osg::Shader *vert_shader = osg::Shader::readShaderFile(osg::Shader::VERTEX,  "default_color.vert");
    osg::Shader *frag_shader = osg::Shader::readShaderFile(osg::Shader::FRAGMENT,"default_color.frag");
    osg::Program *program=new osg::Program();
    program->addShader(vert_shader);
    program->addShader(frag_shader);
    osg::StateSet *ss = model->getOrCreateStateSet();
    ss->setAttributeAndModes(program, osg::StateAttribute::ON);
    ss->setMode(GL_BLEND,osg::StateAttribute::ON);
    ss->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

}

osg::MatrixTransform* createShadowMesh(osg::MatrixTransform *tr, float *shadowMat)
{

	osg::Matrix mt;
	mt.set(shadowMat);
    osg::MatrixTransform *new_mt=new osg::MatrixTransform(*tr);
    mt.preMult(new_mt->getMatrix());    
    new_mt->setMatrix(mt);

    osg::Vec3f pos, scale;
    osg::Quat rot, so;
    
    mt.decompose(pos, rot, scale, so);
    printf("scale %f, %f, %f\n", scale.x(),scale.y(),scale.z());
    printf("pos %f, %f, %f\n", pos.x(),pos.y(),pos.z());

    return new_mt;
}

void addModels(osg::Group *root)
{
    osg::ref_ptr<osg::Node> scene_node = osgDB::readRefNodeFile("scene.osgt");
    if(scene_node==NULL)
    {
        return;
    }
    osg::Group *scene = scene_node->asGroup();
    if(scene_node==NULL)
    {
        return;
    }
    float mat[4][4];
    float new_mat[16];
    float plane[4]={0.0,0.0,1.0,0.002};
    float lightpos[4]={5.0,7.0,10.0,1.0};
    shadowMatrix(mat, plane, lightpos);
    float *p=&mat[0][0];
    float *new_p=&new_mat[0];
    for(int i=0;i<16;i++)
    {
        *new_p=*p;       
        new_p++;p++;
    }
    
    printf("\n");
    for(unsigned int i=0;i<scene->getNumChildren();i++)
    {
        osg::MatrixTransform *model = dynamic_cast<osg::MatrixTransform*> (scene->getChild(i));
        printf("model: %s\n", model->getName().c_str());
        if((model->getName()=="Cube")||(model->getName()=="Sphere"))
        {
            printf("adding shadow mesh\n");
            osg::MatrixTransform *tr = createShadowMesh(model,&new_mat[0]);
            setShader(tr);
            osgUtil::Optimizer optimizer;
            optimizer.optimize(tr);

            root->addChild(tr);
        }
        //else if (model->getName()=="entity_background")
        //    continue;
        //setShader(model);
        root->addChild(model);
    }
    //osgDB::writeNodeFile(*root, "scene_result.osgt");

}

int main()
{
    //osg::setNotifyLevel(osg::INFO);
    osgViewer::Viewer *m_viewer = new osgViewer::Viewer;
    m_viewer->setUpViewInWindow(0,0,600,800);
            
    m_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
    m_viewer->addEventHandler(new osgViewer::StatsHandler);  
    
    
    osg::Group  *root = new osg::Group();
    addModels(root);      
    m_viewer->setSceneData(root);
    
    m_viewer->realize();
    while (!m_viewer->done())
    {
        m_viewer->frame();
    }
    
    return 0;
}    		
