#pragma once

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSet>
#include <iterator>


#include "option_wrapper.h"


namespace Application {
    class PathfindingCommandParser : public QCommandLineParser {
    public:

        static inline PathfindingCommandParser& getInstance(){
            static PathfindingCommandParser instance;
            return instance;
        }

        void addOption(const QCommandLineOption& option, const QStringList &setIds);

        [[nodiscard]] QPair<bool, QString> inputOptionsValid() const;

        PathfindingCommandParser(const PathfindingCommandParser&) = delete;
        PathfindingCommandParser& operator=(const PathfindingCommandParser&) = delete;

    private:
        PathfindingCommandParser();

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

