#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "mavlink.h"

/** @brief Polling interval in ms */
#ifdef MAVLINK_ENABLED_SLUGS
  #define SERIAL_POLL_INTERVAL 7
#else
  #define SERIAL_POLL_INTERVAL 2
#endif

/** @brief Heartbeat emission rate, in Hertz (times per second) */
#define MAVLINK_HEARTBEAT_DEFAULT_RATE 1

#define WITH_TEXT_TO_SPEECH 1

#define QGC_APPLICATION_NAME "QGroundControl"
#define QGC_APPLICATION_VERSION "v. 0.7.7 (Beta)"

#endif // CONFIGURATION_H
