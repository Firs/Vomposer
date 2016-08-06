#pragma once

#include <QQmlPropertyValueSource>
#include <QQmlProperty>
#include <memory>

class FPitch;
class FPitchMonitor;

class QmlPitchValueSource : public QObject, public QQmlPropertyValueSource
{
    Q_OBJECT
    Q_INTERFACES(QQmlPropertyValueSource)

public:
    QmlPitchValueSource(QObject* Parent = nullptr);

    virtual void setTarget(const QQmlProperty& Property);

private slots:
    void OnPitchUpdated(const FPitch* Pitch);

private:
    QQmlProperty TargetProperty;
    std::shared_ptr<FPitchMonitor> PitchMonitor;
};
