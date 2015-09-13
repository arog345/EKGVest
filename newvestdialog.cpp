#include <QtWidgets>

#include "newvestdialog.h"

// Dialog box used when the user wishes to view a new vest.
NewVestDialog::NewVestDialog(QWidget *parent)
    : QDialog(parent)
{
    // The part of the dialog shown by default
    ipLabel = new QLabel(tr("IP Address:"));
    ipEdit = new QLineEdit("127.0.0.1");

    nameLabel = new QLabel(tr("Name (optional):"));
    nameEdit = new QLineEdit;

    findButton = new QPushButton(tr("Connect"));
    findButton->setDefault(true);

    closeButton = new QPushButton(tr("Close"));

    moreButton = new QPushButton(tr("More"));
    moreButton->setCheckable(true);
    moreButton->setAutoDefault(false);

    buttonBox = new QDialogButtonBox(Qt::Vertical);
    buttonBox->addButton(findButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(closeButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(moreButton, QDialogButtonBox::ActionRole);

    // The part shown by clicking the "More" button
    extension = new QWidget;

    portLabel = new QLabel(tr("Port Number:"));
    portEdit = new QLineEdit("1234");

    ekgCheckBox = new QCheckBox(tr("Show EKG"));
    echoCheckBox = new QCheckBox(tr("Show echocardiogram data"));
    rememberCheckBox = new QCheckBox(tr("Remember this vest"));
    ekgCheckBox->setChecked(true);
    echoCheckBox->setChecked(true);
    rememberCheckBox->setChecked(true);

    // Show the extension when the more button is clicked
    connect(moreButton, SIGNAL(toggled(bool)), extension, SLOT(setVisible(bool)));

    // Extension area layout
    QVBoxLayout *extensionLayout = new QVBoxLayout;
    extensionLayout->setMargin(0);

    QHBoxLayout *portLayout = new QHBoxLayout;
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portEdit);

    extensionLayout->addLayout(portLayout);
    extensionLayout->addWidget(ekgCheckBox);
    extensionLayout->addWidget(echoCheckBox);
    extensionLayout->addWidget(rememberCheckBox);
    extension->setLayout(extensionLayout);

    // IP and name layout
    QHBoxLayout *firstLine = new QHBoxLayout;
    firstLine->addWidget(ipLabel);
    firstLine->addWidget(ipEdit);

    QHBoxLayout *secondLine = new QHBoxLayout;
    secondLine->addWidget(nameLabel);
    secondLine->addWidget(nameEdit);

    QVBoxLayout *defaultEdits = new QVBoxLayout;
    defaultEdits->addLayout(firstLine);
    defaultEdits->addLayout(secondLine);

    // Overall layout
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(defaultEdits, 0, 0);
    mainLayout->addWidget(buttonBox, 0, 1);
    mainLayout->addWidget(extension, 1, 0, 1, 2);
    mainLayout->setRowStretch(2,1);

    setLayout(mainLayout);
    setWindowTitle("New Vest");

    // Hide the extension from the user until the more button is clicked
    extension->hide();

    // Set up the signals and slots
    connect(findButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

}

void NewVestDialog::getOptions(QString &ip, QString &name, qint16 &port,
                               bool &ekg, bool &echo, bool &remember)
{
    ip = ipEdit->text();
    name = nameEdit->text();
    port = portEdit->text().toInt();
    ekg = ekgCheckBox->isChecked();
    echo = echoCheckBox->isChecked();
    remember = rememberCheckBox->isChecked();
}

NewVestDialog::~NewVestDialog()
{

}

