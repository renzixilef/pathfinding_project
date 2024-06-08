//
// Created by frenzikowski on 08.06.24.
//

#include "GUI/gui_tab_multi_run.h"

GUI::MultiRunTab::MultiRunTab(QWidget *parent) :
QWidget(parent),
configTable(new QTableWidget(this)),
configLine(new QLineEdit(this)),
mainLayout(new QVBoxLayout(this))
{

}
