#ifndef NEWVESTDIALOG_H
#define NEWVESTDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QDialogButtonBox;
class QGroupBox;
class QLineEdit;
class QPushButton;

class NewVestDialog : public QDialog
{
    Q_OBJECT

public:
    NewVestDialog(QWidget *parent = 0);
    ~NewVestDialog();
    void getOptions(QString &ip, QString &name, qint16 &port,
                    bool &ekg, bool &echo, bool &remember);

private:
    QLabel *ipLabel;
    QLabel *nameLabel;
    QLabel *portLabel;
    QLineEdit *ipEdit;
    QLineEdit *nameEdit;
    QLineEdit *portEdit;
    QCheckBox *ekgCheckBox;
    QCheckBox *echoCheckBox;
    QCheckBox *rememberCheckBox;
    QDialogButtonBox *buttonBox;
    QPushButton *findButton;
    QPushButton *closeButton;
    QPushButton *moreButton;
    QWidget *extension;
};

#endif // NEWVESTDIALOG_H
