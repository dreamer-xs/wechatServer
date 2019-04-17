#ifndef DEVICEPROTOCOL_H
#define DEVICEPROTOCOL_H

#include <QThread>
#include <QHash>

class DeviceProtocol : public QThread
{
    Q_OBJECT
public:
    DeviceProtocol();
    ~DeviceProtocol();
    void run();

signals:

public slots:

protected slots:

private:

};

#endif // DEVICEPROTOCOL_H
