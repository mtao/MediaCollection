#ifndef TAG_H
#define TAG_H

#include <taglib/fileref.h>
#include <QString>
//Wrapper for 
class Tag {
    public:
        Tag(const QString & str = "");

        //TODO: figure out whether storing these datas will hurt performance in however this ends up being used?
        QString title()const;
        QString artist()const;
        QString album()const;
        QString comment()const;
        QString genre()const;
        unsigned int track()const;

    private:
        QString m_filepath;
        TagLib::FileRef m_fileref;//Have to keep this around so m_tag doesn't stale.
        TagLib::Tag * m_tag;


};
#endif
