#pragma once
#include <QString>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <concepts>
#include <qobject.h>
#include <type_traits>

class ILabWindow
{
public:
    virtual ~ILabWindow() = default; // To not to destruct object from base class. To avoid memory leak because base class won't destruct derived class
    virtual QString get_name(void) const = 0;
};

namespace
{
    template<typename Work, typename WorkRet>
    concept ThreadFunction = std::same_as<std::invoke_result_t<Work>, WorkRet>;
}

template<typename WorkRet>
class Threaded
{
public:
    Threaded(QWidget* parent = nullptr){ watcher = new QFutureWatcher<WorkRet>(parent); }

protected:
    QFutureWatcher<WorkRet>* watcher;

    // Use lambda for worker and ret.
    // Possible to use reference in ret.
    template<
        typename Reciever,
        typename Work,
        typename Ret,
        typename Control,
        typename Signal
            >
    requires ThreadFunction<Work, WorkRet>
    void to_thread(Reciever reciever,
                   Control control, Signal signal,
                   Work    worker,  Ret    ret)
    {
        QObject::connect(watcher, &QFutureWatcher<WorkRet>::finished, reciever, ret);
        QObject::connect(control, signal, reciever, [reciever, worker](){
                auto future = QtConcurrent::run(worker);
                reciever->watcher->setFuture(future);
                });
    }
};
