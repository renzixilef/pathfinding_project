#include "GUI/widgets/gui_run_progress_view.h"

GUI::Widgets::RunProgressView::RunProgressView(QWidget *parent) : QWidget(parent),
                                                                  layout(new QVBoxLayout(this)),
                                                                  scrollArea(new QScrollArea()),
                                                                  scrollWidget(new QWidget()),
                                                                  scrollAreaLayout(new QVBoxLayout(scrollWidget)){
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    layout->addWidget(scrollArea);
    setLayout(layout);
}

void GUI::Widgets::RunProgressView::addNewConfig(const QString &name) {
    auto *nameLabel = new QLabel(name);
    auto *progressBar = new QProgressBar();
    auto benchLayout = new QVBoxLayout();
    benchLayout->addWidget(nameLabel);
    benchLayout->addWidget(progressBar);
    scrollAreaLayout->addLayout(benchLayout);
    progressBars[name] = progressBar;
}

void GUI::Widgets::RunProgressView::updateProgress(const QString &name, uint16_t progress) {
    if (progressBars.contains(name)) {
        progressBars[name]->setValue(progress);
    }
}
