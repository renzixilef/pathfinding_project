#pragma once

#include <QCommandLineOption>
#include <QObject>
#include <QHash>

namespace Application {

    class OptionWrapper {
    public:
        OptionWrapper(const QCommandLineOption &option) : option(option) {}

        const QCommandLineOption &getOption() const { return option; }

        bool operator==(const OptionWrapper &other) const {
            return option.names() == other.option.names();
        }

    private:
        const QCommandLineOption &option;
    };

    inline uint32_t qHash(const OptionWrapper &key, uint32_t seed = 0) {
        return qHash(key.getOption().names().first(), seed);
    }
}


