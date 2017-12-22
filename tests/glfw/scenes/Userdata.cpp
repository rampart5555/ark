#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osg/io_utils>
#include <osg/ArgumentParser>
#include <osg/UserDataContainer>

#include <osg/ValueObject>

#include "Userdata.h"

typedef struct 
{
    int ep_id;
    std::string ep_name;
    bool ep_unlocked;        

}EpisodeInfo;

typedef struct 
{
    int lvl_id;
    bool lvl_unlocked;
    int lvl_score;
    std::string lvl_data;

}LevelInfo;


static void dump_episode_info(EpisodeInfo ei)
{
    printf("Episode\n");
    printf("ei_id: %d\n",ei.ep_id);
    printf("ei_name: %s\n",ei.ep_name.c_str());
    printf("ei_unlocked: %d\n",ei.ep_unlocked);
}

static void dump_level_info(LevelInfo li)
{
    printf("  Level\n");
    printf("    lvl_id: %d\n",li.lvl_id);
    printf("    lvl_unlocked: %d\n",li.lvl_unlocked);
    printf("    lvl_score: %d\n",li.lvl_score);
    printf("    lvl_data: %s\n",li.lvl_data.c_str());
}

class FindLevel : public osg::NodeVisitor
{
    public:            
        FindLevel(): osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
        {
                        
        } 
        void apply(osg::Group& node)
        {   
            EpisodeInfo ei;
            LevelInfo li;  
            bool res=false;
            res = node.getUserValue("ep_id",ei.ep_id);
            if(res==true)
            {
                node.getUserValue("ep_name", ei.ep_name);
                node.getUserValue("ep_unlocked", ei.ep_unlocked);
                dump_episode_info(ei);                
            }
            res = node.getUserValue("lvl_id",li.lvl_id);
            if(res==true)
            {
                node.getUserValue("lvl_unlocked",li.lvl_unlocked);
                node.getUserValue("lvl_score",li.lvl_score);
                node.getUserValue("lvl_data",li.lvl_data);        
                dump_level_info(li);
            }                
            
            traverse(node);
        }
};



static void write_user_data()
{
    osg::Group *root = new osg::Group();
    for(int i=0;i<3;i++)
    {
        std::string ep_name="Episode_"+i;
        osg::Group *episode = new osg::Group();
        episode->setUserValue("ep_id",i);
        episode->setUserValue("ep_name",ep_name);
        episode->setUserValue("ep_unlocked",false);
        for(int j=0;j<15;j++)
        {
            char buf[10];
            osg::Group *level = new osg::Group();
            level->setUserValue("lvl_id",i);
            level->setUserValue("lvl_unlocked",false);
            level->setUserValue("lvl_score",0);
            std::string lvl_data = "levels/level_";
            sprintf(buf,"%02d",i+1);
            lvl_data += buf;
            lvl_data+=".lua";
            level->setUserValue("lvl_data",lvl_data);
            episode->addChild(level);
        }
        root->addChild(episode);
    }    
    osgDB::writeNodeFile(*root, "levels.osgt");
}

static void read_user_data()
{
    osg::Node *node = osgDB::readNodeFile("levels.osgt");
    if(node==NULL)
    {
        printf("Invalid node file\n");
        return;
    }    
    FindLevel fl;
    node->accept(fl);    
}

Userdata::Userdata()
{

}

Userdata::~Userdata()
{

}

void Userdata::createScene()
{
#if 0    
    osg::Group *episode=new osg::Group();
    episode->setUserValue("ep_id",1);
    episode->setUserValue("ep_name",std::string("Episode_1"));
    episode->setUserValue("ep_unloked",true);
    for(int i=0;i<15;i++)
    {
        char buf[10];
        osg::Group *level = new osg::Group();
        level->setUserValue("lvl_id",i);
        level->setUserValue("lvl_unlocked",false);
        level->setUserValue("lvl_score",0);
        std::string lvl_data = "levels/level_";
        sprintf(buf,"%02d",i+1);
        lvl_data += buf;
        lvl_data+=".lua";
        level->setUserValue("lvl_data",lvl_data);
        episode->addChild(level);
    }
    osgDB::writeNodeFile(*episode, "levels.osgt");
#endif
    write_user_data();
    read_user_data();
}
