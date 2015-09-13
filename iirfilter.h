#ifndef IIRFILTER_H
#define IIRFILTER_H

#include <QObject>

#define SOS_ROWS_notch 7
#define SOS_ROWS_bandpass 10

class IIRFilter : public QObject
{
    Q_OBJECT
public:
    explicit IIRFilter(QObject *parent = 0);
    ~IIRFilter();

    double recentOutput();

    // Notch filter coefficients
    static const double SOS_notch[SOS_ROWS_notch][6];
    static const double G_notch[SOS_ROWS_notch+1];

    // Bandpass filters coefficients
    static const double SOS_bandpass[SOS_ROWS_bandpass][6];
    static const double G_bandpass[SOS_ROWS_bandpass+1];

signals:
    void newOutput(double output);

public slots:
    void filterInput(double input);

private:
    double iir_buffer_notch[SOS_ROWS_notch][6];
    double output_notch[SOS_ROWS_notch];

    double iir_buffer_bandpass[SOS_ROWS_bandpass][6];
    double output_bandpass[SOS_ROWS_bandpass];

    double notchFilter(double input);
    double bandpassFilter(double input);

    double output;


};

#endif // IIRFILTER_H
