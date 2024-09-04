#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QObject>
#include <QSystemTrayIcon>

class notification : public QObject
{
    Q_OBJECT

public:
    notification();

    void notification_ajoutOffre();
    void notification_modifierOffre();
    void notification_supprimerOffre();
    void notification_failed();

private:
    void showNotification(const QString &title, const QString &message, QSystemTrayIcon::MessageIcon iconType);
};

#endif // NOTIFICATION_H
