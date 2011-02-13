/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "haze-yahoo-account.h"
#include <KDebug>

class HazeYahooAccount::Private
{
public:
    Private()
    {
        kDebug();
    }
};

HazeYahooAccount::HazeYahooAccount(QObject* parent)
        : AbstractAccountUi(parent), d(new Private)
{
    kDebug();

    // only these two parameters are supported at the moment,
    // I'll add more once I determine which are important
    registerSupportedParameter("account", QVariant::String);
    registerSupportedParameter("password", QVariant::String);
}

HazeYahooAccount::~HazeYahooAccount()
{
    kDebug();
    delete d;
}

bool HazeYahooAccount::hasAdvancedOptionsWidget() const
{
    return false;
}

AbstractAccountParametersWidget* HazeYahooAccount::mainOptionsWidget(
	ParameterEditModel* model,
	QWidget* parent) const
{
    return new YahooMainOptionsWidget(model,parent);
}

AbstractAccountParametersWidget* HazeYahooAccount::advancedOptionsWidget(
	ParameterEditModel* model, 
	QWidget* parent) const
{
    return NULL;
}
