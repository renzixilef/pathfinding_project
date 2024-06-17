#include "GUI/widgets/gui_tab_single_run.h"
#include "Pathfinder/pathfinding.h"
#include "GridGenerator/obstacle_gen.h"
#include "GUI/gui_dialog_single_run.h"


GUI::Widgets::SingleRunTab::SingleRunTab(QWidget *parent) :
        QWidget(parent),
        configForm(new SingleConfigForm(this)),
        layout(new QVBoxLayout(this)),
        startRunButton(new QPushButton("Run", this)) {

    connect(startRunButton, &QPushButton::clicked, this, &SingleRunTab::startRun);

    layout->addWidget(configForm);
    layout->addWidget(startRunButton);
    setLayout(layout);
}

void GUI::Widgets::SingleRunTab::startRun() {
    auto configParams = configForm->getFormParams();

    QDialog *singleRunDialog = new GUI::SingleRunDialog(configParams.first, *configParams.second.begin());
    singleRunDialog->exec();

}
