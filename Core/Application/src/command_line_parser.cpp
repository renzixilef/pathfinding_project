#include "command_line_parser.h"


void Application::PathfindingCommandParser::addOption(const QCommandLineOption &option, const QStringList &setIds) {
    for (const auto &id: setIds) optionSets[id].insert(OptionWrapper(option));
    QCommandLineParser::addOption(option);
}

QPair<bool, QString> Application::PathfindingCommandParser::inputOptionsValid() const {
    QSet<QString> sharedSets;
    bool isFirstOption = true;

    for (auto it = optionSets.constBegin(); it != optionSets.constEnd(); it++) {
        QSet<QString> currentOptionSets;
        for (const OptionWrapper &opt: it.value()) {
            if (this->isSet(opt.getOption())) {
                currentOptionSets.insert(it.key());
                if (!isFirstOption && !sharedSets.intersects(currentOptionSets)) {
                    return qMakePair(false, opt.getOption().names().first());
                }
            }
        }
        if (isFirstOption) {
            sharedSets.swap(currentOptionSets);
            isFirstOption = false;
        } else {
            sharedSets.intersect(currentOptionSets);
        }
    }
    return qMakePair(true, QString());
}
