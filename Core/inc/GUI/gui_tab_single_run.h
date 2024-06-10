#pragma once
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>

#include "gui_config_form.h"

namespace GUI {

    class SingleRunTab : public QWidget{
        Q_OBJECT
    public:
        explicit SingleRunTab(QWidget *parent = nullptr);


    private:
        void startRun();

        QPushButton* startRunButton;
        SingleConfigForm* configForm;

        QVBoxLayout *layout;
    };

}