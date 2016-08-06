#include "QmlPitchValueSource.h"

#include "PitchMonitor.h"
#include "Utils.h"

QmlPitchValueSource::QmlPitchValueSource(QObject* Parent)
    : QObject(Parent)
    , PitchMonitor(new FPitchMonitor(this))
{
    CHECKED_CONNECT(PitchMonitor.get(), SIGNAL(PitchDetected(const FPitch*)),
                    this, SLOT(OnPitchUpdated(const FPitch*)));

    PitchMonitor->Start();
}

void QmlPitchValueSource::OnPitchUpdated(const FPitch* Pitch)
{
    if (Pitch)
    {
        TargetProperty.write(Pitch->ToString());
    }
    else
    {
        TargetProperty.write("");
    }
}

void QmlPitchValueSource::setTarget(const QQmlProperty& Property)
{
    TargetProperty = Property;
}
