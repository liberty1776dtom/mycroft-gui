#include "skillloader.h"

#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>

#include <config-mycroft.h>

SkillLoader::SkillLoader(QObject *parent):
    QObject(parent)
{

}

SkillLoader::~SkillLoader()
{
}

QString SkillLoader::uiForMetadataType(const QString &metaDataType) const
{
    const QString filePath = MYCROFT_SKILLS_UI_DIR + QLatin1String("/") + metaDataType + "/Main.qml";
    qDebug() << "query" << filePath;
    if (QFileInfo::exists(filePath)) {
        return  QUrl::fromLocalFile(filePath).toString();
    }
    qWarning() << "could not find delegate for " << metaDataType;
    return QString();
}
