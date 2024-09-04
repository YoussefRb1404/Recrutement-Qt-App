#include "notification.h"

notification::notification()
{
    // Constructor implementation
}

void notification::notification_ajoutOffre()
{
    showNotification("Gestion des offres", "Nouvelle offre ajoutée", QSystemTrayIcon::Information);
}

void notification::notification_modifierOffre()
{
    showNotification("Gestion des offres", "Offre modifiée", QSystemTrayIcon::Information);
}

void notification::notification_supprimerOffre()
{
    showNotification("Gestion des offres", "Offre supprimée", QSystemTrayIcon::Information);
}

void notification::notification_failed()
{
    showNotification("Gestion des offres", "Action échouée", QSystemTrayIcon::Critical);
}

void notification::showNotification(const QString &title, const QString &message, QSystemTrayIcon::MessageIcon iconType)
{
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
    notifyIcon->setIcon(QIcon(":/new/prefix1/haha.jpg"));
    notifyIcon->show();
    notifyIcon->showMessage(title, message, iconType, 15000);
}
