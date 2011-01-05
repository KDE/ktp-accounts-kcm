#ifndef PROTOCOLPARAMETERVALUE_H
#define PROTOCOLPARAMETERVALUE_H

#include "TelepathyQt4/ProtocolParameter"
#include <kdemacros.h>

class KDE_EXPORT ProtocolParameterValue
{
public:
    ProtocolParameterValue(const Tp::ProtocolParameter &parameter, const QVariant value);
    ProtocolParameterValue(const ProtocolParameterValue &other);
    ~ProtocolParameterValue();
    ProtocolParameterValue &operator=(const ProtocolParameterValue &other);

    const Tp::ProtocolParameter parameter() const;
    const QString name() const;
    const QVariant value() const;
    /** returns true if the value is different from the parameter default and is non-null*/
    bool shouldSave() const ;

private:
    class Private;
    Private *d;
};

#endif // PROTOCOLPARAMETERVALUE_H
