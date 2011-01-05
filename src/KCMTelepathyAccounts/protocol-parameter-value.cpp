#include "protocol-parameter-value.h"

class ProtocolParameterValue::Private
{
public:
    Tp::ProtocolParameter parameter;
    QVariant value;
};


ProtocolParameterValue::ProtocolParameterValue(const Tp::ProtocolParameter &parameter, const QVariant value)
    :d (new Private)
{
    d->parameter = parameter;
    d->value = value;
    //make sure we're storing the correct type of QVariant type for this parameter
    d->value.convert(d->parameter.type());
}

ProtocolParameterValue::ProtocolParameterValue(const ProtocolParameterValue &other)
    :d (new Private(*other.d))
{

}

ProtocolParameterValue::~ProtocolParameterValue()
{
    delete d;
}

ProtocolParameterValue &ProtocolParameterValue::operator=(const ProtocolParameterValue &other)
{
    *d = *other.d;
    return *this;
}


const Tp::ProtocolParameter ProtocolParameterValue::parameter() const
{
    return d->parameter;
}

const QVariant ProtocolParameterValue::value() const
{
    return d->value;
}

const QString ProtocolParameterValue::name() const
{
    QString name;
    if(d->parameter.isValid()) {
        name = d->parameter.name();
    }
    return name;
}

bool ProtocolParameterValue::shouldSave() const
{
    if (! d->parameter.isValid()) {
        return false;
    }

    if (! d->value.isValid()) {
        return false;
    }

    if (d->value.isNull()) {
        return false;
    }

    // Don't any parameters where the default value is equal to the current value.
    if (d->parameter.defaultValue() == d->value) {
        return false;
    }

    // Don't save any strings where the default is empty, and the value is an empty string
    if (d->parameter.type() == QVariant::String) {
        if ((d->parameter.defaultValue().isNull()) && d->value.toString().isEmpty()) {
            return false;
        }
    }

    return true;
}
