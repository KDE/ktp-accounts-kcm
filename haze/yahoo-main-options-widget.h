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


#ifndef HAZE_YAHOO_ACCOUNT_H
#define HAZE_YAHOO_ACCOUNT_H

#include "ui_yahoo-main-options-widget.h"

#include <KCMTelepathyAccounts/AbstractAccountParametersWidget>

class YahooMainOptionsWidget
  : public AbstractAccountParametersWidget
{
  Q_OBJECT
  
  public:
    YahooMainOptionsWidget(ParameterEditModel* model, QWidget* parent = 0);
    virtual ~YahooMainOptionsWidget();
    
private:
  Q_DISABLE_COPY(YahooMainOptionsWidget);
  
  class Private;
    Private * const d;
};

#endif // HAZE_YAHOO_ACCOUNT_H
