#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QDialogButtonBox;
class QGroupBox;
class QLineEdit;
class QPushButton;

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void getOptions(qint16 ekgPort, qint16 echoPort, qint16 discoveryPort,
                    bool ekgEnable, bool echoEnable);

signals:

public slots:

private:
    QLabel *ekgPortLabel;
    QLabel *echoPortLabel;
    QLabel *discoveryListenPortLabel;
    QLabel *discoverySendPortLabel;
    QLineEdit *ekgPortEdit;
    QLineEdit *echoPortEdit;
    QLineEdit *discoveryListenPortEdit;
    QLineEdit *discoverySendPortEdit;
    QCheckBox *ekgDefaultCheckBox;
    QCheckBox *echoDefaultCheckBox;
    QDialogButtonBox *buttonBox;
    QPushButton *saveButton;
    QPushButton *closeButton;
};

#endif // SETTINGSDIALOG_H
