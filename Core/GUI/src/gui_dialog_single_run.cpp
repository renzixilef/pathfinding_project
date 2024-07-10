#include "gui_dialog_single_run.h"

#include <QScreen>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <algorithm>
#include <QMenu>
#include <QLabel>
#include <QtConcurrentRun>
#include <QFutureWatcher>

GUI::SingleRunDialog::SingleRunDialog(const RunInterface::SingleRunConfig &runConfig,
                                      QWidget *parent) :
        QDialog(parent),
        runInterface(new RunInterface::SingleRun(runConfig)),
        singleRunThread(new QThread(this)),
        nextStepButton(new QPushButton("Next Step")),
        toggleRunButton(new QPushButton("Play")),
        exportRunMenuButton(new QToolButton()),
        toggleStartEndRedefinitionButton(new QPushButton("Choose Start/End")),
        gridWidget(new Widgets::GridDrawerWidget(runInterface->getGridRef())),
        liveEvaluator(new Widgets::LiveEvaluatorWidget(runInterface->getSolverRef())),
        mainLayout(new QVBoxLayout(this)),
        gridWidgetLayout(new QHBoxLayout()),
        buttonLayout(new QHBoxLayout()),
        nextStepTimer(new QTimer(this)) {

    qRegisterMetaType<std::string>("std::string");
    showMaximized();
    setWindowTitle("Pathfinder 1.0 - Single Run");

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
    gridWidgetLayout->addWidget(liveEvaluator);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addLayout(gridWidgetLayout);

    setLayout(mainLayout);
}

void GUI::SingleRunDialog::onStepFinished() {
    gridWidget->update();
    liveEvaluator->updateMetrics();
    gridWidget->enqueueNextPixmap();
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
    liveEvaluator->updateMetrics();
    gridWidget->enqueueNextPixmap();
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

    if (!filenameStd.empty()) {
        emit serialize(filenameStd);
        toggleRunButton->setDisabled(true);
    }

}

void GUI::SingleRunDialog::onSaveDone() {
    toggleRunButton->setEnabled(true);
}

void GUI::SingleRunDialog::toggleStartEndRedefinitionButtonHandler() {
    if (gridWidget->getRedefinitionStatus()) {
        toggleStartEndRedefinitionButton->setStyleSheet("");
        toggleStartEndRedefinitionButton->setText("Choose Start/End");
        gridWidget->toggleStartEndRedefinitionPhase();
        toggleRunButton->setEnabled(true);
        nextStepButton->setEnabled(true);
        if (!exportRunMenuButton->isHidden()) {
            exportRunMenuButton->setEnabled(true);
        }
        emit startEndChanged();
        gridWidget->resetPixmapQueue();
    } else {
        toggleStartEndRedefinitionButton->setText("Done");
        toggleStartEndRedefinitionButton->setStyleSheet("background-color: red;");
        gridWidget->toggleStartEndRedefinitionPhase();
        toggleRunButton->setDisabled(true);
        nextStepButton->setDisabled(true);
        if (!exportRunMenuButton->isHidden()) {
            exportRunMenuButton->setDisabled(true);
        }
        QMessageBox redefinitionExplainerBox;
        redefinitionExplainerBox.setWindowTitle("Info");
        redefinitionExplainerBox.setMinimumWidth(800);
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

    if (!filenameStd.empty()) {
        auto waitForVideoExportBox = new QMessageBox;
        waitForVideoExportBox->setAttribute(Qt::WA_DeleteOnClose, true);
        waitForVideoExportBox->setWindowModality(Qt::ApplicationModal);
        waitForVideoExportBox->setText("Please wait...");
        waitForVideoExportBox->setStandardButtons(QMessageBox::NoButton);
        waitForVideoExportBox->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
        waitForVideoExportBox->show();
        QObject::connect(&exportVideoWatcher, &QFutureWatcher<void>::finished,
                         waitForVideoExportBox, &QMessageBox::accept);
        exportVideoFuture = QtConcurrent::run([this, filenameStd]() {
            gridWidget->exportPixmapQueue(filenameStd);
        });
        exportVideoWatcher.setFuture(exportVideoFuture);

    }
}

void GUI::SingleRunDialog::closeEvent(QCloseEvent *event) {
    runInterface->terminate();
    QThread::msleep(1000);
    singleRunThread->quit();
    if (!singleRunThread->wait(3000)) {
        singleRunThread->terminate();
        singleRunThread->wait();
    }
    QDialog::closeEvent(event);
}
