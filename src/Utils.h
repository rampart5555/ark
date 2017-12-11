
#include <string>
#include <osg/Object>


class CharBuffer: public osg::Object
{
    public:
        CharBuffer();        
        virtual const char* libraryName() const {return "";}
        virtual const char* className() const {return "";}
        virtual osg::Object* cloneType() const {return NULL;}
        virtual osg::Object* clone(const osg::CopyOp&) const {return NULL;}

        bool createFromFile(const char*);
        const char* getData();
        unsigned int getDataSize();
        
    private:
        ~CharBuffer();
        char *m_data;
        unsigned int m_size;
        std::string m_file;
};