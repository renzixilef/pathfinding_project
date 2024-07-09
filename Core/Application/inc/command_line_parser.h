#pragma once

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSet>
#include <QDebug>
#include <iterator>
#include <variant>


#include "option_wrapper.h"
#include "runner.h"


namespace Application {

    enum class UIType {
        UI_TYPE_GUI,
        UI_TYPE_HEADLESS
    };

    enum class HeadlessConfigInputType {
        INPUT_JSON_PATH,
        INPUT_COMMAND_LINE_FLAGS,
    };

    class PathfindingCommandParser : public QCommandLineParser {
    public:
        static inline PathfindingCommandParser &getInstance() {
            static PathfindingCommandParser instance;
            return instance;
        }

        [[nodiscard]] QPair<bool, QString> inputOptionsValid() const;

        [[nodiscard]] UIType getUIType() const;

        [[nodiscard]] std::optional<HeadlessConfigInputType> getConfigInputType() const;

        [[nodiscard]] std::variant<RunInterface::MultiRunConfig, QString> getRunConfig() const;

        [[nodiscard]] std::variant<std::list<RunInterface::MultiRunConfig>, QString> parseJSONConfig();

        PathfindingCommandParser(const PathfindingCommandParser &) = delete;

        PathfindingCommandParser &operator=(const PathfindingCommandParser &) = delete;

    private:
        PathfindingCommandParser();

        inline static void printDefaultingToMessage(const QCommandLineOption& option, const QString&& defaultValue){
            qDebug() << option.names() << " not set. Defaulting to: " << defaultValue;
        }

        void addOption(const QCommandLineOption &option, const QStringList &setIds);

        static std::optional<QStringList> parseWithRegex(const QString &str, QRegularExpression &&regex);

        static std::variant<RunInterface::RunGridConfig, QString>
        parseJSONGridConfig(const QJsonObject &gridConfigJson);

        static std::variant<std::list<Pathfinder::PathfinderStrategy>, QString>
        parseJSONStrats(const QJsonArray &stratsJson);

        QHash<QString, QSet<OptionWrapper>> optionSets;

        QCommandLineOption guiOption;
        QCommandLineOption headlessOption;
        QCommandLineOption headlessJSONConfigOption;
        QCommandLineOption gridDimensionsOption;
        QCommandLineOption obstacleGenOption;
        QCommandLineOption obstacleDensityOption;
        QCommandLineOption minStartEndOption;
        QCommandLineOption solverOption;
        QCommandLineOption iterationsOption;
    };

}

