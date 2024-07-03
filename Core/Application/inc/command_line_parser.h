#pragma once

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSet>
#include <iterator>
#include <variant>


#include "option_wrapper.h"
#include "runner.h"


namespace Application {

    enum class UIType{
        UI_TYPE_GUI,
        UI_TYPE_HEADLESS
    };

    enum class HeadlessConfigInputType{
        INPUT_JSON_PATH,
        INPUT_COMMAND_LINE_FLAGS,
    };

    class PathfindingCommandParser : public QCommandLineParser {
    public:

        static inline PathfindingCommandParser& getInstance(){
            static PathfindingCommandParser instance;
            return instance;
        }

        [[nodiscard]] QPair<bool, QString> inputOptionsValid() const;

        [[nodiscard]] UIType getUIType() const;

        [[nodiscard]] std::optional<HeadlessConfigInputType> getConfigInputType() const;

        [[nodiscard]] std::optional<RunInterface::RunGridConfig> getRunConfig() const;

        PathfindingCommandParser(const PathfindingCommandParser&) = delete;
        PathfindingCommandParser& operator=(const PathfindingCommandParser&) = delete;

    private:
        PathfindingCommandParser();
        void addOption(const QCommandLineOption& option, const QStringList &setIds);

        QHash<QString, QSet<OptionWrapper>> optionSets;

        QCommandLineOption guiOption;
        QCommandLineOption headlessOption;
        QCommandLineOption headlessJSONConfigOption;
        QCommandLineOption gridDimensionsOption;
        QCommandLineOption obstacleGenOption;
        QCommandLineOption obstacleDensityOption;
        QCommandLineOption minStartEndOption;
        QCommandLineOption solverOption;
    };

}
