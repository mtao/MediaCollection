#include "../include/tag.h"
#include <taglib/tstring.h>
#include <QFileInfo>

#include <QDebug>

Tag::Tag(const QString & str)
    : m_filepath(str)
    , m_fileref(str.toUtf8().data())
    , m_tag(m_fileref.tag())
{
    qWarning() << "Tag: " << m_tag;
}



QString Tag::title() const{
    if(!m_tag || m_tag->isEmpty()) return QString();
    return TStringToQString(m_tag->title());
}
QString Tag::artist() const{
    if(!m_tag || m_tag->isEmpty()) return QString();
    return TStringToQString(m_tag->artist());
}
QString Tag::comment() const{
    if(!m_tag || m_tag->isEmpty()) return QString();
    return TStringToQString(m_tag->comment());
}
QString Tag::genre() const{
    if(!m_tag || m_tag->isEmpty()) return QString();
    return TStringToQString(m_tag->genre());
}
QString Tag::album() const{
    if(!m_tag || m_tag->isEmpty()) return QString();
    return TStringToQString(m_tag->album());
}
unsigned int Tag::track() const{
    if(!m_tag || m_tag->isEmpty()) return 0;
    return m_tag->track();
}
