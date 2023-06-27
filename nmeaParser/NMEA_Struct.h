#ifndef PARSENMEA_H
#define PARSENMEA_H

#include <string.h>
#include <stdbool.h>
#include <stdint.h>

struct GPGGA{
    double   time;
    double   latitude;
    char     lat;
    double   longitude;
    char     lon;
    uint8_t  observation;
    uint8_t  nka;
    double   geo_factor;
    double   H_antena;
    char     h_antena;
    double   H_geoid;
    char     h_geoid;
    uint16_t diff;
};

struct POHPR{
    double  time;
    float   course;
    float   roll;
    float   pitch;
    char    status;
};

struct POUGT{
    uint8_t   satellite;
    uint8_t   param;
    char      status;
    uint32_t  time;
    float     Course;
    float     X1;
    float     X2;
    float     base_line;
    float     diff;
    float     base_satellite;
    uint8_t   R;
};

struct PONDM{
    float Course;
    float Pitch;
    float Roll;
    float Distance_1;
    float Distance_2;
    float Distance_3;
};

struct PORZA{
    uint8_t     Number;
    uint32_t    Boudrate;
    uint8_t     Proto;
};

struct PNVGO{
    bool        Signal;
    float       Pitch;
    float       PitchI;
};

struct PNVGVER{
    bool        Signal;
    char        PNVGVER_Soft[11];
    char        PNVGVER_NSoft_1[5];
    char        PNVGVER_NSoft_2[5];
    char        PNVGVER_NSoft_3[5];
    char        PNVGVER_Linux_Kernel[30];
};

struct PNVGS{
    bool        Signal;
    float       Loss;
    float       Seach;
};

struct PNVGR{
    uint8_t      Status;
    float        Ahez;
};

struct GPRMC{
    float       Time;
    char        Status;
    float       Lat;
    float       Long;
    float       Speed;
    float       Course;
    uint32_t    Date;
    float       Magnit;
    char        Magnit_Varitation;
    char        Mode;
    char        Navigation_Status;
};

struct PORZV{
    char    Stat;
    int     Time;
    int     Day;
    int     Mounth;
    int     Year;
};

struct GPZDA{
    char k;
};

struct NMEA_Data{
    struct GPGGA    GPGGA;
    struct POHPR    POHPR;
    struct POUGT    POUGT;
    struct PONDM    PONDM;
    struct PORZA    PORZA;
    struct PNVGO    PNVGO;
    struct PNVGVER  PNVGVER;
    struct PNVGS    PNVGS;
    struct PNVGR    PNVGR;
    struct GPRMC    RMC;
    struct PORZV    PORZV;
    struct GPZDA    GPZDA;
};

typedef struct {
    union {
        uint8_t nfields;
        char    data[82];
    };
    char       *cursor;
} BmNmeaMessage;

typedef struct {

    char manufacturer_name[16];
    char device_name[16];

    BmNmeaMessage rx;

} BmUartProtoNmea;

#ifdef __cplusplus

extern "C" {

void nmea_recv(void *, const char *, size_t , struct NMEA_Data *);

unsigned NmeaFormat(char *dst, unsigned dstlen, const char *fmt, ...);
}

#endif

#endif // PARSENMEA_H
