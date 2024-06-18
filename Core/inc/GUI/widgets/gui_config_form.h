#pragma once

#include <QSpinBox>
#include <QComboBox>
#include <QFormLayout>
#include <QListWidget>
#include <QLabel>

#include "RunInterface/runner.h"
#include "Pathfinder/pathfinding.h"

namespace GUI::Widgets {

    class ConfigFormParent : public QWidget {
    Q_OBJECT
    public:
        explicit ConfigFormParent(QWidget *parent = nullptr);

        virtual std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>> getFormParams() = 0;

        virtual void resetForm();

        virtual void enable();

        virtual void disable();

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

        void resetForm() override;

        void enable() override;

        void disable() override;


    private:
        QComboBox *pathfindingAlgorithmComboBox;
    };

    class MultiConfigForm : public ConfigFormParent {
    Q_OBJECT
    public:
        explicit MultiConfigForm(QWidget *parent = nullptr);

        std::pair<RunInterface::RunGridConfig, std::list<Pathfinder::PathfinderStrategy>> getFormParams() override;

        void resetForm() override;

        void enable() override;

        void disable() override;

        bool inputValid();

        void handleInvalidInput();

        void populate(const RunInterface::RunGridConfig& config, const std::list<Pathfinder::PathfinderStrategy>& strats);

    private:
        QLabel *invalidInputWarningLabel;
        QListWidget *pathfindingAlgorithmListWidget;
        QSpinBox *iterationsSpinBox;
    };

}