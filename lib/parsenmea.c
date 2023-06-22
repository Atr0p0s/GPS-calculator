#include "NMEA_Struct.h"
#include "stdlib.h"
#include<stdarg.h>

# define ARRAY_SIZE(a)                  (sizeof(a)/sizeof(a)[0])

typedef void BmNmeaRxHandler(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data);

static double stof(const char* s){
  double rez = 0, fact = 1;

  if (*s == '-')
  {
    s++;
    fact = -1;
  };

  int point_seen = 0;
  for (point_seen = 0; *s; s++)
  {
    if (*s == '.')
    {
      point_seen = 1;
      continue;
    };

    int d = *s - '0';

    if (d >= 0 && d <= 9)
    {
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (double)d;
    };
  }

  return rez * fact;
}

static double toGrad(const double Grad_Min)
{
    unsigned latit = (unsigned)Grad_Min;

    double L = Grad_Min - latit;

    L = L * 5/3;

    L += latit;

    return L;
}

static void rx_gga(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->GPGGA.time)
    {
        NMEA_Data->GPGGA.time = 0;

        int i = strlen(msg->data) + 1;
        NMEA_Data->GPGGA.time           = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.latitude       = stof(&msg->data[i])/100.0;

        NMEA_Data->GPGGA.latitude = toGrad(NMEA_Data->GPGGA.latitude);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.lat            = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.longitude      = stof(&msg->data[i])/100.0;

        NMEA_Data->GPGGA.longitude = toGrad(NMEA_Data->GPGGA.longitude);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.lon            = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.observation 	= (uint8_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.nka            = (uint8_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.geo_factor     = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.H_antena       = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.h_antena       = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.H_geoid        = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.h_geoid        = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->GPGGA.diff           = (uint16_t)atoi(&msg->data[i]);
    }
}

static void rx_rmc(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->RMC.Time)
    {
        NMEA_Data->RMC.Time = 0;

        int i = strlen(msg->data) + 1;
        NMEA_Data->RMC.Time = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Status = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Lat = stof(&msg->data[i]);

        NMEA_Data->RMC.Lat = toGrad(NMEA_Data->RMC.Lat);

        i += strlen(&msg->data[i]) + 1;
        if(msg->data[i] == 'S')
            NMEA_Data->RMC.Lat *= (-1);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Long = stof(&msg->data[i]);

        NMEA_Data->RMC.Long = toGrad(NMEA_Data->RMC.Long);

        i += strlen(&msg->data[i]) + 1;
        if(msg->data[i] == 'W')
            NMEA_Data->RMC.Long *= (-1);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Speed = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Course = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Date = atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Magnit = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Magnit_Varitation = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Mode = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->RMC.Navigation_Status = msg->data[i];
    }
}

static void rx_pohpr(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->POHPR.time)
    {
        NMEA_Data->POHPR.time = 0;

        int i = strlen(msg->data) + 1;
        NMEA_Data->POHPR.time       = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POHPR.course     = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POHPR.roll       = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POHPR.pitch      = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POHPR.status     = msg->data[i];
    }
}

static void rx_pougt(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->POUGT.satellite)
    {
        NMEA_Data->POUGT.satellite = 0;

        int i = strlen(msg->data) + 1;
        NMEA_Data->POUGT.satellite      = (uint8_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.param          = (uint8_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.status         = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.time           = (uint32_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.Course         = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.X1             = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.X2             = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.base_line      = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.diff           = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->POUGT.base_satellite = stof(&msg->data[i]);
    }
}

static void rx_podst(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{

}

static void rx_pondm(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->PONDM.Course)
    {
        NMEA_Data->PONDM.Course = 0;

        int i = strlen(msg->data) + 1;
        NMEA_Data->PONDM.Course         = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PONDM.Roll          = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PONDM.Pitch           = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PONDM.Distance_1     = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PONDM.Distance_2     = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PONDM.Distance_3     = stof(&msg->data[i]);
    }
}

static void rx_porza(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->PORZA.Number)
    {
        NMEA_Data->PORZA.Number = 0;

        int i = strlen(msg->data) + 1;
        NMEA_Data->PORZA.Number         = (uint8_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PORZA.Boudrate       = (uint32_t)atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PORZA.Proto          = (uint8_t)atoi(&msg->data[i]);
    }
}

static void rx_porzb(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{

}

static void rx_pongl(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{

}

static void rx_pnvgo(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->PNVGO.Signal)
    {
        NMEA_Data->PNVGO.Signal = 0;

        NMEA_Data->PNVGO.Signal = true;

        int i = strlen(msg->data) + 1;
        NMEA_Data->PNVGO.Pitch          = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PNVGO.PitchI         = stof(&msg->data[i]);
    }
}


static void rx_pnvgver(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->PNVGVER.Signal)
    {
        NMEA_Data->PNVGVER.Signal = 0;

        NMEA_Data->PNVGVER.Signal = true;

        int i = strlen(msg->data) + 1;
        strncpy(NMEA_Data->PNVGVER.PNVGVER_Soft, &msg->data[i], 11);

        i += strlen(&msg->data[i]) + 1;
        strncpy(NMEA_Data->PNVGVER.PNVGVER_NSoft_1, &msg->data[i], 5);

        i += strlen(&msg->data[i]) + 1;
        strncpy(NMEA_Data->PNVGVER.PNVGVER_NSoft_2, &msg->data[i], 5);

        i += strlen(&msg->data[i]) + 1;
        strncpy(NMEA_Data->PNVGVER.PNVGVER_NSoft_3, &msg->data[i], 5);

        i += strlen(&msg->data[i]) + 1;
        strncpy(NMEA_Data->PNVGVER.PNVGVER_Linux_Kernel, &msg->data[i], 30);
    }
}

static void rx_pnvgs(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->PNVGS.Signal)
    {
        NMEA_Data->PNVGS.Signal = 0;

        NMEA_Data->PNVGS.Signal = true;

        int i = strlen(msg->data) + 1;
        NMEA_Data->PNVGS.Loss          = stof(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PNVGS.Seach         = stof(&msg->data[i]);
    }
}

static void rx_pnvgr(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    if(NMEA_Data->PNVGR.Status)
    {
        NMEA_Data->PNVGR.Status = 0;
        int i = strlen(msg->data) + 1;
        NMEA_Data->PNVGR.Status          = (uint8_t)atoi(&msg->data[i]);
    }
}

static void rx_porzv(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
        int i = strlen(msg->data) + 1;
        NMEA_Data->PORZV.Stat = msg->data[i];

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PORZV.Time = atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PORZV.Day = atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PORZV.Mounth = atoi(&msg->data[i]);

        i += strlen(&msg->data[i]) + 1;
        NMEA_Data->PORZV.Year = atoi(&msg->data[i]);
}

static void rx_gpzda(BmNmeaMessage *msg, struct NMEA_Data *NMEA_Data)
{
    NMEA_Data->GPZDA.k = 1;
}


static struct {
    const char *templ;
    BmNmeaRxHandler *handler;
} rx_handlers[] = {
    {"**GGA",   rx_gga },
    {"**RMC",   rx_rmc },
    {"POHPR",   rx_pohpr},
    {"POUGT",   rx_pougt},
    {"PODST",   rx_podst},
    {"PONDM",   rx_pondm},
    {"PORZA",   rx_porza},
    {"PORZB",   rx_porzb},
    {"PONGL",   rx_pongl},
    {"PNVGO",   rx_pnvgo},
    {"PNVGVER", rx_pnvgver},
    {"PNVGS",   rx_pnvgs},
    {"PNVGR",   rx_pnvgr},
    {"PORZV",   rx_porzv},
    {"**ZDA",   rx_gpzda}
};

static bool nmea_parse(BmNmeaMessage *msg) {
    char *p = msg->data;
    if (*p != '$')
        return false;

    unsigned x = 0;
    msg->nfields = 0;
    while (*++p != '*') {
        unsigned byte = *(char *) p;
        if (byte > 127 || byte < 32)
            return false;
        x ^= byte;
        if (*p == ',') {
            *p = 0;
            msg->nfields++;
        }
    }
    *p = 0;
    if (*++p != "0123456789ABCDEF"[x >> 4])
        return false;
    if (*++p != "0123456789ABCDEF"[x & 15])
        return false;

    msg->cursor = msg->data + 1;
    return *++p == '\r';
}


void nmea_recv(void *ctx, const char *data, size_t size, struct NMEA_Data *NMEA_Data)
{
    BmUartProtoNmea *nmea = ctx;

    BmNmeaMessage *msg = &nmea->rx;

    for (; size--; ++data) {
        if (*data == '$')
            msg->cursor = msg->data;
        if (msg->cursor == msg->data + sizeof msg->data)
            msg->cursor = NULL;
        if (msg->cursor == NULL)
            continue;
        *msg->cursor++ = *(const char *)data;
        if (*data != '\n')
            continue;
        if (nmea_parse(msg)) {

            size_t i;
            for (i = 0; i < ARRAY_SIZE(rx_handlers); ++i)
            {
                const char *tpl = rx_handlers[i].templ;
                size_t len = strlen(tpl);
                if (len != strlen(msg->cursor))
                    continue;
                size_t j;
                for (j = 0; j < len; ++j)
                    if (tpl[j] != '*' && msg->cursor[j] != tpl[j])
                        goto next;
                rx_handlers[i].handler(msg, NMEA_Data);
                next:;
            }
        }
        msg->cursor = NULL;
    }
}

unsigned NmeaFormat(char *dst, unsigned dstlen, const char *fmt, ...) {
    if (!dst || !fmt || dstlen < 11)
        return 0;
    va_list args;
    va_start(args, fmt);
    *dst++ = '$';
    int n = vsnprintf(dst, dstlen - 6, fmt, args);
    va_end(args);

    if (n < 0 || (unsigned) n >= dstlen - 6)
        return 0;

    int crc = 0;
    for (int i = 1; i <= n; ++i)
        crc ^= *dst++;
    sprintf(dst, "*%02hhX\r\n", crc);
    if ((unsigned) n >= dstlen - 6)
        dst[5] = 0;
    return (unsigned) n + 6;
}
