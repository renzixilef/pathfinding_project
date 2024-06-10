#pragma once

#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>

#include "RunInterface/runner.h"
#include "Pathfinder/pathfinding.h"

namespace GUI {

    class ConfigFormParent : public QWidget {
    Q_OBJECT
    public:
        explicit ConfigFormParent(QWidget *parent = nullptr);

        virtual std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>> getFormParams() = 0;

        virtual void resetForm() = 0;

        virtual void enable() = 0;

        virtual void disable() = 0;

    protected:
        QSpinBox *gridHeightSpinBox;
        QSpinBox *gridWidthSpinBox;
        QDoubleSpinBox *obstacleDensitySpinBox;
        QDoubleSpinBox *minStartEndDistanceSpinBox;
        QComboBox *gridGeneratorAlgorithmComboBox;
        QFormLayout *layout;
    };

    class SingleConfigForm : public ConfigFormParent {
    Q_OBJECT
    public:
        explicit SingleConfigForm(QWidget *parent = nullptr);

        std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>> getFormParams() override;

        void resetForm() override {};

        void enable() override {};

        void disable() override {};

    private:
        QComboBox *pathfindingAlgorithmComboBox;
    };

    class MultiConfigForm : public ConfigFormParent {
    Q_OBJECT
    public:
        explicit MultiConfigForm(QWidget *parent = nullptr);

    private:
    };

}