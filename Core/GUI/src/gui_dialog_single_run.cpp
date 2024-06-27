#include "gui_dialog_single_run.h"

#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>
#include <QMenu>
#include <QLabel>

GUI::SingleRunDialog::SingleRunDialog(const RunInterface::RunGridConfig &config,
                                      const Pathfinder::PathfinderStrategy &strat,
                                      QWidget *parent) :
        QDialog(parent),
        runInterface(new RunInterface::SingleRun(config, strat)),
        singleRunThread(new QThread(this)),
        nextStepButton(new QPushButton("Next Step")),
        toggleRunButton(new QPushButton("Play")),
        exportRunMenuButton(new QToolButton()),
        toggleStartEndRedefinitionButton(new QPushButton("Choose Start/End")),
        gridWidget(new Widgets::GridDrawerWidget(runInterface->getGridRef())),
        mainLayout(new QVBoxLayout(this)),
        gridWidgetLayout(new QHBoxLayout()),
        buttonLayout(new QHBoxLayout()),
        nextStepTimer(new QTimer(this)) {

    qRegisterMetaType<std::string>("std::string");
    showMaximized();

    toggleRunButton->setStyleSheet("background-color: green; color: white;");

    exportRunMenuButton->hide();
    exportRunMenuButton->setText("Export Run");
    auto *exportMenu = new QMenu();
    exportVideoAction = exportMenu->addAction("Export Video");
    debugSerializeAction = exportMenu->addAction("Serialize Grid");
    exportRunMenuButton->setMenu(exportMenu);
    exportRunMenuButton->setPopupMode(QToolButton::MenuButtonPopup);

    runInterface->moveToThread(singleRunThread);

    setupConnections();

    singleRunThread->start();

    buttonLayout->addWidget(toggleRunButton);
    buttonLayout->addWidget(nextStepButton);
    buttonLayout->addWidget(toggleStartEndRedefinitionButton);
    buttonLayout->addWidget(exportRunMenuButton);
    gridWidgetLayout->addWidget(gridWidget);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gridWidgetLayout);

    setLayout(mainLayout);
}

void GUI::SingleRunDialog::onStepFinished() {
    gridWidget->update();
    if (!runPaused) {
        nextStepTimer->setSingleShot(true);
        nextStepTimer->start(50);
    } else {
        nextStepButton->setEnabled(true);
        toggleRunButton->setEnabled(true);
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green; color: white;");
    }
}

void GUI::SingleRunDialog::toggleRunButtonHandler() {
    if (runFinished) {
        runPaused = true;
        runFinished = false;
        exportRunMenuButton->hide();
        toggleStartEndRedefinitionButton->setEnabled(true);
        toggleRunButton->setText("Play");
        toggleRunButton->setStyleSheet("background-color: green; color: white;");
        toggleRunButton->setEnabled(false);
        gridWidget->resetPixmapQueue();
        emit resetRun();
    } else {
        if (runPaused) {
            toggleStartEndRedefinitionButton->setDisabled(true);
            nextStepButton->setEnabled(false);
            toggleRunButton->setText("Pause");
            toggleRunButton->setStyleSheet("background-color: red; color: white;");
            runPaused = false;
            emit nextStep();
        } else {
            toggleStartEndRedefinitionButton->setDisabled(true);
            runPaused = true;
        }
    }
}

void GUI::SingleRunDialog::nextStepButtonHandler() {
    nextStepButton->setEnabled(false);
    toggleRunButton->setEnabled(false);
    toggleStartEndRedefinitionButton->setDisabled(true);
    emit nextStep();
}

void GUI::SingleRunDialog::onGridFinished() {
    gridWidget->update();
    nextStepButton->setEnabled(false);
    toggleRunButton->setEnabled(true);
    toggleRunButton->setText("New Run");
    toggleRunButton->setStyleSheet("background-color: blue; color: white;");
    exportRunMenuButton->show();
    runFinished = true;
}

void GUI::SingleRunDialog::setupConnections() {
    connect(this, SIGNAL(nextStep()),
            runInterface, SLOT(nextStep()));
    connect(this, SIGNAL(resetRun()),
            runInterface, SLOT(onRunReset()));
    connect(runInterface, SIGNAL(stepFinished()),
            this, SLOT(onStepFinished()));
    connect(runInterface, SIGNAL(gridFinished()),
            this, SLOT(onGridFinished()));
    connect(singleRunThread, SIGNAL(finished()),
            runInterface, SLOT(deleteLater()));
    connect(nextStepButton, &QPushButton::clicked,
            this, &SingleRunDialog::nextStepButtonHandler);
    connect(toggleRunButton, &QPushButton::clicked,
            this, &SingleRunDialog::toggleRunButtonHandler);
    connect(debugSerializeAction, &QAction::triggered,
            this, &SingleRunDialog::serializeButtonHandler);
    connect(exportVideoAction, &QAction::triggered,
            this, &SingleRunDialog::exportVideoHandler);
    connect(toggleStartEndRedefinitionButton, &QPushButton::clicked,
            this, &SingleRunDialog::toggleStartEndRedefinitionButtonHandler);
    connect(this, SIGNAL(serialize(const std::string&)),
            runInterface, SLOT(onSerializeRequest(const std::string&)));
    connect(this, SIGNAL(startEndChanged()),
            runInterface, SLOT(onStartEndChanged()));
    connect(runInterface, SIGNAL(saveDone()),
            this, SLOT(onSaveDone()));
    connect(nextStepTimer, &QTimer::timeout,
            this, &SingleRunDialog::nextStep);

}

void GUI::SingleRunDialog::serializeButtonHandler() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Grid As"),
                                                    "/home", tr("Binary Files (*.bin);; All Files(*)"));
    std::string filenameStd = filename.toStdString();

    if(!filenameStd.empty()){
        emit serialize(filenameStd);
        toggleRunButton->setDisabled(true);
    }

}

void GUI::SingleRunDialog::onSaveDone() {
    toggleRunButton->setEnabled(true);
}

void GUI::SingleRunDialog::toggleStartEndRedefinitionButtonHandler() {
    if(gridWidget->getRedefinitionStatus()){
        gridWidget->toggleStartEndRedefinitionPhase();
        toggleRunButton->setEnabled(true);
        nextStepButton->setEnabled(true);
        if(!exportRunMenuButton->isHidden()){
            exportRunMenuButton->setEnabled(true);
        }
        emit startEndChanged();
    }else{
        gridWidget->toggleStartEndRedefinitionPhase();
        toggleRunButton->setDisabled(true);
        nextStepButton->setDisabled(true);
        if(!exportRunMenuButton->isHidden()){
            exportRunMenuButton->setDisabled(true);
        }
        QMessageBox redefinitionExplainerBox;
        redefinitionExplainerBox.setText("How to redefine Start/End");
        redefinitionExplainerBox.setInformativeText("Set start position: Left Click on the new node\n"
                                                    "Set end position: Right Click on the new node");
        redefinitionExplainerBox.exec();
    }
}

void GUI::SingleRunDialog::exportVideoHandler() {
    QString filename = QFileDialog::getSaveFileName(this, tr("Export Video as"),
                                                    "/home", tr("AVI Files (*.avi);; All Files(*)"));
    std::string filenameStd = filename.toStdString();

    if(!filenameStd.empty()){
        QDialog dialog(this);
        dialog.setModal(true);
        auto *label = new QLabel("Please Wait");
        auto *layout = new QVBoxLayout();
        layout->addWidget(label);
        dialog.setLayout(layout);
        dialog.setWindowFlags(Qt::Dialog | Qt::WindowTitleHint);
        dialog.show();
        gridWidget->exportPixmapQueue(filenameStd);
        dialog.accept();
    }
}
