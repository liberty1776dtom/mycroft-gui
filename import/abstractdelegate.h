/*
 *   Copyright 2018 by Marco Martin <mart@kde.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#pragma once

#include <QQuickItem>

#include <QQmlPropertyMap>
#include <QPointer>

#include "sessiondatamap.h"

class MycroftController;

class AbstractDelegate: public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(SessionDataMap *sessionData READ sessionData CONSTANT)
    Q_PROPERTY(int timeout MEMBER m_timeout NOTIFY timeoutChanged)

    //TODO: api for background? would like to avoid
    Q_PROPERTY(int leftPadding MEMBER m_leftPadding NOTIFY leftPaddingChanged)
    Q_PROPERTY(int rightPadding MEMBER m_rightPadding NOTIFY rightPaddingChanged)
    Q_PROPERTY(int topPadding MEMBER m_topPadding NOTIFY topPaddingChanged)
    Q_PROPERTY(int bottomPadding MEMBER m_bottomPadding NOTIFY bottomPaddingChanged)

public:
    AbstractDelegate(QQuickItem *parent = nullptr);
    ~AbstractDelegate();

    //API used only by AbstractSkillView during initialization, *NOT* QML
    //void setController(MycroftController *controller);
    void setSessionData(SessionDataMap *data);
    SessionDataMap *sessionData() const;

    /**
     * INTERNAL: Url of the qml file that generated this instance
     */
    void setQmlUrl(const QUrl &url);
    QUrl qmlUrl() const;

    /**
     * @internal skill id this delegate belongs to
     */
    void setSkillId(const QString &skillId);
    QString skillId() const;

Q_SIGNALS:
    /**
     * Emitted when the delegate asks to be the "current" in the view
     *TODO: maybe abuse focus instead?
     */
    void currentRequested();

    /**
     * Emitted when the server triggered an event.
     * It is guaranteed the event will be either a system event or an event belonging to our skill, but never to another skill
     * @param eventName the unique name for the event
     * @param data the data for this event in JSon form
     */
    void event(const QString &eventName, const QVariantMap &data);

    //QML property notifiers
    void timeoutChanged();
    void leftPaddingChanged();
    void rightPaddingChanged();
    void topPaddingChanged();
    void bottomPaddingChanged();

private:
    QPointer<SessionDataMap> m_data;

    QUrl m_qmlUrl;
    QString m_skillId;

    int m_timeout = 5000; //Completely arbitrary 5 seconds of timeout

    int m_leftPadding = 0; //FIXME: how to bind to kirigami units from c++?
    int m_rightPadding = 0;
    int m_topPadding = 0;
    int m_bottomPadding = 0;
};
