#include "SlugsMAV.h"

#include <QDebug>

SlugsMAV::SlugsMAV(MAVLinkProtocol* mavlink, int id) :
        UAS(mavlink, id)//,
        // Place other initializers here
{
  widgetTimer = new QTimer (this);
  widgetTimer->setInterval(SLUGS_UPDATE_RATE);

  connect (widgetTimer, SIGNAL(timeout()),
           this, SLOT(emitSignals()));

  widgetTimer->start();

  // clear all the state structures
   memset(&mlRawImuData ,0, sizeof(mavlink_raw_imu_t));

#ifdef MAVLINK_ENABLED_SLUGS


    memset(&mlGpsData, 0, sizeof(mavlink_gps_raw_t));
    memset(&mlCpuLoadData, 0, sizeof(mavlink_cpu_load_t));
    memset(&mlAirData, 0, sizeof(mavlink_air_data_t));
    memset(&mlSensorBiasData, 0, sizeof(mavlink_sensor_bias_t));
    memset(&mlDiagnosticData, 0, sizeof(mavlink_diagnostic_t));
    memset(&mlPilotConsoleData, 0, sizeof(mavlink_pilot_console_t));
    memset(&mlFilteredData ,0, sizeof(mavlink_filtered_data_t));
    memset(&mlBoot ,0, sizeof(mavlink_boot_t));
    memset(&mlGpsDateTime ,0, sizeof(mavlink_gps_date_time_t));
    memset(&mlApMode ,0, sizeof(mavlink_set_mode_t));
    memset(&mlPwmCommands ,0, sizeof(mavlink_pwm_commands_t));
    memset(&mlPidValues ,0, sizeof(mavlink_pid_values_t));
    memset(&mlSinglePid ,0, sizeof(mavlink_pid_t));
    memset(&mlNavigation ,0, sizeof(mavlink_slugs_navigation_t));
    memset(&mlDataLog ,0, sizeof(mavlink_data_log_t));
    memset(&mlPassthrough ,0, sizeof(mavlink_ctrl_srfc_pt_t));
    memset(&mlActionAck,0, sizeof(mavlink_action_ack_t));
    memset(&mlAction ,0, sizeof(mavlink_slugs_action_t));


    updateRoundRobin = 0;



    uasId = id;

  #endif
}

/**
 * This function is called by MAVLink once a complete, uncorrupted (CRC check valid)
 * mavlink packet is received.
 *
 * @param link Hardware link the message came from (e.g. /dev/ttyUSB0 or UDP port).
 *             messages can be sent back to the system via this link
 * @param message MAVLink message, as received from the MAVLink protocol stack
 */
void SlugsMAV::receiveMessage(LinkInterface* link, mavlink_message_t message)
{
    // Let UAS handle the default message set
   UAS::receiveMessage(link, message);


    // Handle your special messages mavlink_message_t* msg = &message;
    switch (message.msgid)
    {

#ifdef MAVLINK_ENABLED_SLUGS

      case MAVLINK_MSG_ID_BOOT:
        mavlink_msg_boot_decode(&message,&mlBoot);
        emit slugsBootMsg(uasId, mlBoot);
      break;

      case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_msg_attitude_decode(&message, &mlAttitude);
      break;

      case MAVLINK_MSG_ID_GPS_RAW:
        mavlink_msg_gps_raw_decode(&message, &mlGpsData);
            break;

      case MAVLINK_MSG_ID_ACTION_ACK:      // 62
        mavlink_msg_action_ack_decode(&message,&mlActionAck);
            break;

      case MAVLINK_MSG_ID_CPU_LOAD:       //170
        mavlink_msg_cpu_load_decode(&message,&mlCpuLoadData);
            break;

      case MAVLINK_MSG_ID_AIR_DATA:       //171
        mavlink_msg_air_data_decode(&message,&mlAirData);
            break;

      case MAVLINK_MSG_ID_SENSOR_BIAS:    //172
        mavlink_msg_sensor_bias_decode(&message,&mlSensorBiasData);
      break;

      case MAVLINK_MSG_ID_DIAGNOSTIC:     //173
        mavlink_msg_diagnostic_decode(&message,&mlDiagnosticData);
      break;

      case MAVLINK_MSG_ID_PILOT_CONSOLE:  //174
        mavlink_msg_pilot_console_decode(&message,&mlPilotConsoleData);
            break;

      case MAVLINK_MSG_ID_PWM_COMMANDS:   //175
        mavlink_msg_pwm_commands_decode(&message,&mlPwmCommands);
      break;

      case MAVLINK_MSG_ID_SLUGS_NAVIGATION://176
        mavlink_msg_slugs_navigation_decode(&message,&mlNavigation);
            break;

      case MAVLINK_MSG_ID_DATA_LOG:       //177
        mavlink_msg_data_log_decode(&message,&mlDataLog);
      break;

      case MAVLINK_MSG_ID_FILTERED_DATA:  //178
        mavlink_msg_filtered_data_decode(&message,&mlFilteredData);
      break;

      case MAVLINK_MSG_ID_GPS_DATE_TIME:    //179
        mavlink_msg_gps_date_time_decode(&message,&mlGpsDateTime);
            break;

      case MAVLINK_MSG_ID_MID_LVL_CMDS:     //180

      break;

      case MAVLINK_MSG_ID_CTRL_SRFC_PT:     //181

            break;

      case MAVLINK_MSG_ID_PID:              //182
          memset(&mlSinglePid,0,sizeof(mavlink_pid_t));
          mavlink_msg_pid_decode(&message, &mlSinglePid);
          qDebug() << "\nSLUGS RECEIVED PID Message = "<<mlSinglePid.idx;

          emit slugsPidValues(uasId, mlSinglePid);


      break;

      case MAVLINK_MSG_ID_SLUGS_ACTION:     //183

      break;

#endif

      default:
//        qDebug() << "\nSLUGS RECEIVED MESSAGE WITH ID" << message.msgid;
            break;
        }
}



void SlugsMAV::emitSignals (void){
#ifdef MAVLINK_ENABLED_SLUGS
  switch(updateRoundRobin){
    case 1:
      emit slugsCPULoad(uasId, mlCpuLoadData);
      emit slugsSensorBias(uasId,mlSensorBiasData);
    break;

    case 2:
      emit slugsAirData(uasId, mlAirData);
      emit slugsDiagnostic(uasId,mlDiagnosticData);
    break;

    case 3:
      emit slugsPilotConsolePWM(uasId,mlPilotConsoleData);
      emit slugsPWM(uasId, mlPwmCommands);
    break;

    case 4:
      emit slugsNavegation(uasId, mlNavigation);
      emit slugsDataLog(uasId, mlDataLog);
    break;

    case 5:
      emit slugsFilteredData(uasId,mlFilteredData);
      emit slugsGPSDateTime(uasId, mlGpsDateTime);
    break;

    case 6:
      emit slugsActionAck(uasId,mlActionAck);
      emit emitGpsSignals();
    break;
        }

  emit slugsAttitude(uasId, mlAttitude);
  emit attitudeChanged(this,
                       mlAttitude.roll,
                       mlAttitude.pitch,
                       mlAttitude.yaw,
                       0.0);
#endif

  emit slugsRawImu(uasId, mlRawImuData);


  // wrap around
  updateRoundRobin = updateRoundRobin > 10? 1: updateRoundRobin + 1;


}

#ifdef MAVLINK_ENABLED_SLUGS
void SlugsMAV::emitGpsSignals (void){

  if (mlGpsData.fix_type > 0){
    emit globalPositionChanged(this,
                               mlGpsData.lon,
                               mlGpsData.lat,
                               mlGpsData.alt,
                               0.0);

    // Smaller than threshold and not NaN
    if (mlGpsData.v < 1000000 && mlGpsData.v == mlGpsData.v){
      emit speedChanged(this, (double)mlGpsData.v, 0.0, 0.0, 0.0);
    } else {
      emit textMessageReceived(uasId, uasId, 255, QString("GCS ERROR: RECEIVED INVALID SPEED OF %1 m/s").arg(mlGpsData.v));
    }
    }
}

void SlugsMAV::emitPidSignal()
{
     qDebug() << "\nSLUGS RECEIVED PID Message";
    emit slugsPidValues(uasId, mlSinglePid);

}

#endif // MAVLINK_ENABLED_SLUGS
