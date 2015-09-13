#include "iirfilter.h"

#include <QDebug>

const double IIRFilter::SOS_notch[SOS_ROWS_notch][6] = {
    {1,-1.999996318071756,1, 1,-1.999991152016390,0.999994946278235},
    {1,-1.999996579447539,1,1,-1.999991953919885,0.999995273714311},
    {1,-1.999996336459353,1,1,-1.999973850111933,0.999977693396201},
    {1,-1.999996562430257,1,1,-1.999976109034431,0.999979386198291},
    {1,-1.999996405093024,1,1,-1.999899293968296,0.999903300116607},
    {1,-1.999996495734824,1,1,-1.999911066749505,0.999914210080213},
    {1,-1.999996450891303,1,1,-1.999434498913618,0.999438047025100}};

const double IIRFilter::G_notch[SOS_ROWS_notch+1] = {
    0.472816758114194,
    0.472816758114194,
    0.999989267471757,
    0.999989267471757,
    0.999954371216335,
    0.999954371216335,
    4.471879675747826,
    1.000000000000000};

const double IIRFilter::SOS_bandpass[SOS_ROWS_bandpass][6] = {
    {1,1.785136974348379,1,1,1.554563781620487,0.9468638891983137},
    {1,-1.999956515846841,1,1,-1.99907453583108,0.9991603259419275},
    {1,1.841993663192504,1,1,1.376307513974557,0.8680131353787484},
    {1,-1.999968495681395,1,1,-1.997634649514676,0.9977506168627998},
    {1,1.922568247073426,1,1,0.9066321437822817,0.6695262624857817},
    {1,-1.99998487823127,1,1,-1.992698439878575,0.9929244789789282},
    {1,1.989654436786695,1,1,-1.958555972029082,0.9595635676694321},
    {1,-1.9999980135653,1,1,-0.1788087914379118,0.2220188388949713},
    {1,1.758308142479589,1,1,1.625802683126905,0.9855001284936893},
    {1,-1.99995073710423,1,1,-1.999701479726173,0.9997777705939867}};

const double IIRFilter::G_bandpass[SOS_ROWS_bandpass+1] = {
    1.024284990810213,
    1.024284990810213,
    1.253978150968957,
    1.253978150968957,
    1.629727988140173,
    1.629727988140173,
    2.96337906259716,
    2.96337906259716,
    0.05344613600761122,
    0.05344613600761122,
    1};

IIRFilter::IIRFilter(QObject *parent) :
    QObject(parent), output(0)
{
    for (int i = 0; i < SOS_ROWS_notch; i++)
    {
        for (int j = 0; j < 6; j++)
            iir_buffer_notch[i][j] = 0;
    }

    for (int i = 0; i < SOS_ROWS_bandpass; i++)
    {
        for (int j = 0; j < 6; j++)
            iir_buffer_bandpass[i][j] = 0;
    }

    for (int i = 0; i < SOS_ROWS_notch; i++)
        output_notch[i] = 0;

    for (int i = 0; i < SOS_ROWS_bandpass; i++)
        output_bandpass[i] = 0;
}

IIRFilter::~IIRFilter()
{

}

void IIRFilter::filterInput(double input)
{
    output = bandpassFilter(notchFilter(input));
    emit newOutput(output);
}

double IIRFilter::notchFilter(double input)
{
    for(int i = 0; i < SOS_ROWS_notch; i++)
    {
        iir_buffer_notch[i][2] = iir_buffer_notch[i][1];
        iir_buffer_notch[i][1] = iir_buffer_notch[i][0];

        if (i == 0)
            iir_buffer_notch[i][0] = input;
        else
            iir_buffer_notch[i][0] = output_notch[i-1];

        iir_buffer_notch[i][4] = iir_buffer_notch[i][3];
        iir_buffer_notch[i][3] = output_notch[i];

        output_notch[i] = (G_notch[i]*(SOS_notch[i][0]*iir_buffer_notch[i][0]+SOS_notch[i][1]*
                iir_buffer_notch[i][1]+SOS_notch[i][2]*iir_buffer_notch[i][2])-
                SOS_notch[i][4]*iir_buffer_notch[i][3]-SOS_notch[i][5]*
                iir_buffer_notch[i][4])/SOS_notch[i][3];

    }

    return output_notch[SOS_ROWS_notch-1];
}

double IIRFilter::bandpassFilter(double input)
{
    for(int j = 0; j < SOS_ROWS_bandpass; j++)
    {
        iir_buffer_bandpass[j][2] = iir_buffer_bandpass[j][1];
        iir_buffer_bandpass[j][1] = iir_buffer_bandpass[j][0];

        if(j == 0)
            iir_buffer_bandpass[j][0] = input;
        else
            iir_buffer_bandpass[j][0] = output_bandpass[j-1];

        iir_buffer_bandpass[j][4] = iir_buffer_bandpass[j][3];
        iir_buffer_bandpass[j][3] = output_bandpass[j];

        output_bandpass[j] = (G_bandpass[j]*(SOS_bandpass[j][0]*iir_buffer_bandpass[j][0]+
                SOS_bandpass[j][1]*iir_buffer_bandpass[j][1]+SOS_bandpass[j][2]*
                iir_buffer_bandpass[j][2])-SOS_bandpass[j][4]*iir_buffer_bandpass[j][3]-
                SOS_bandpass[j][5]*iir_buffer_bandpass[j][4])/SOS_bandpass[j][3];
    }

    output = output_bandpass[SOS_ROWS_bandpass-1];
    return output;
}

double IIRFilter::recentOutput()
{
    return output;
}


