#pragma once
#include <QWidget>
#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>
#include <QPushButton>

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
        QDoubleSpinBox* minStartEndDistanceSpinBox;
        QComboBox* gridGeneratorAlgorithmComboBox;
        QComboBox* pathfindingAlgorithmComboBox;
        QFormLayout* layout;
    };

}