#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QProgressBar>
#include <QLabel>
#include <QMap>

namespace GUI::Widgets {
    class RunProgressView : public QWidget{
        Q_OBJECT

    public:
        explicit RunProgressView(QWidget *parent = nullptr);

        void addNewConfig(const QString &name);

        void updateProgress(const QString &name, uint16_t progress);

    private:
        QVBoxLayout *layout;
        QScrollArea *scrollArea;
        QWidget *scrollWidget;
        QMap<QString, QProgressBar*> progressBars;
    };
}