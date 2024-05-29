#pragma once
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>

#include "obstacle_gen.h"

namespace GUI {

    class SingleRunTab : public QWidget{
        Q_OBJECT
    public:
        explicit SingleRunTab(QWidget *parent = nullptr);


    private:
        void startRun();

        QPushButton* startRunButton;

        QSpinBox* gridHeightSpinBox;
        QSpinBox* gridWidthSpinBox;
        QDoubleSpinBox* obstacleDensitySpinBox;
        QComboBox* gridGeneratorAlgorithmComboBox;
        QComboBox* pathfindingAlgorithmComboBox;
        QFormLayout* layout;
    };

}