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


#include "yahoo-main-options-widget.h"

#include <KDebug>

class YahooMainOptionsWidget::Private
{
public:
  
  Ui::YahooMainOptionsWidget* ui;
  
    Private()
      : ui(NULL)
    {
        kDebug();
    }
    
    ~Private()
    {
      delete ui;
    }
};

YahooMainOptionsWidget::YahooMainOptionsWidget(ParameterEditModel* model, QWidget* parent)
  : AbstractAccountParametersWidget(model, parent), d(new Private)
{
  kDebug();
  
  //setup the Ui
  d->ui = new Ui::YahooMainOptionsWidget;
  d->ui->setupUi(this);
  
  //map the widgets to their data
  handleParameter("account", QVariant::String, d->ui->accountLineEdit, d->ui->accountLabel);
  handleParameter("password", QVariant::String, d->ui->passwordLineEdit, d->ui->passwordLabel);
}

YahooMainOptionsWidget::~YahooMainOptionsWidget()
{
  kDebug();
  
  delete d;
}
