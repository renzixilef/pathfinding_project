#pragma once

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QSet>
#include <iterator>


#include "option_wrapper.h"


namespace Application {
    class PathfindingCommandParser : public QCommandLineParser {
    public:
        PathfindingCommandParser():QCommandLineParser(){}

        void addOption(const QCommandLineOption& option, const QStringList &setIds);

        [[nodiscard]] QPair<bool, QString> inputOptionsValid() const;

    private:
        QHash<QString, QSet<OptionWrapper>> optionSets;
    };

}

