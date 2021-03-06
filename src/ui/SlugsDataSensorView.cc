#include "SlugsDataSensorView.h"
#include "ui_SlugsDataSensorView.h"

#include <UASManager.h>
#include "SlugsMAV.h"

#include <QDebug>

SlugsDataSensorView::SlugsDataSensorView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SlugsDataSensorView)
{
    ui->setupUi(this);

    activeUAS = NULL;

    this->setVisible(false);




}

SlugsDataSensorView::~SlugsDataSensorView()
{
    delete ui;
}

void SlugsDataSensorView::addUAS(UASInterface* uas)
{
    SlugsMAV* slugsMav = dynamic_cast<SlugsMAV*>(uas);

  if (slugsMav != NULL) {

    connect(slugsMav, SIGNAL(slugsRawImu(int, const mavlink_raw_imu_t&)), this, SLOT(slugRawDataChanged(int, const mavlink_raw_imu_t&)));

    #ifdef MAVLINK_ENABLED_SLUGS

        //connect standar messages
    connect(slugsMav, SIGNAL(localPositionChanged(UASInterface*,double,double,double,quint64)), this, SLOT(slugLocalPositionChanged(UASInterface*,double,double,double,quint64)));
    connect(slugsMav, SIGNAL(speedChanged(UASInterface*,double,double,double,quint64)), this, SLOT(slugSpeedLocalPositionChanged(UASInterface*,double,double,double,quint64)));
    connect(slugsMav, SIGNAL(attitudeChanged(UASInterface*,double,double,double,quint64)), this, SLOT(slugAttitudeChanged(UASInterface*,double,double,double,quint64)));
    connect(slugsMav, SIGNAL(globalPositionChanged(UASInterface*,double,double,double,quint64)), this, SLOT(slugsGlobalPositionChanged(UASInterface*,double,double,double,quint64)));



        //connect slugs especial messages
    connect(slugsMav, SIGNAL(slugsSensorBias(int,const mavlink_sensor_bias_t&)), this, SLOT(slugsSensorBiasChanged(int,const mavlink_sensor_bias_t&)));
    connect(slugsMav, SIGNAL(slugsDiagnostic(int,const mavlink_diagnostic_t&)), this, SLOT(slugsDiagnosticMessageChanged(int,const mavlink_diagnostic_t&)));
    connect(slugsMav, SIGNAL(slugsCPULoad(int,const mavlink_cpu_load_t&)), this, SLOT(slugsCpuLoadChanged(int,const mavlink_cpu_load_t&)));
    connect(slugsMav, SIGNAL(slugsNavegation(int,const mavlink_slugs_navigation_t&)),this,SLOT(slugsNavegationChanged(int,const mavlink_slugs_navigation_t&)));
    connect(slugsMav, SIGNAL(slugsDataLog(int,const mavlink_data_log_t&)), this, SLOT(slugsDataLogChanged(int,const mavlink_data_log_t&)));
    connect(slugsMav, SIGNAL(slugsPWM(int,const mavlink_pwm_commands_t&)),this,SLOT(slugsPWMChanged(int,const mavlink_pwm_commands_t&)));
    connect(slugsMav, SIGNAL(slugsFilteredData(int,const mavlink_filtered_data_t&)),this,SLOT(slugsFilteredDataChanged(int,const mavlink_filtered_data_t&)));
    connect(slugsMav, SIGNAL(slugsGPSDateTime(int,const mavlink_gps_date_time_t&)),this,SLOT(slugsGPSDateTimeChanged(int,const mavlink_gps_date_time_t&)));

    #endif // MAVLINK_ENABLED_SLUGS
        // Set this UAS as active if it is the first one
    if(activeUAS == 0) {
            activeUAS = uas;
        }

    }
}

void SlugsDataSensorView::slugRawDataChanged(int uasId, const mavlink_raw_imu_t &rawData){
 Q_UNUSED(uasId);

 ui->m_Axr->setText(QString::number(rawData.xacc));
 ui->m_Ayr->setText(QString::number(rawData.yacc));
 ui->m_Azr->setText(QString::number(rawData.zacc));
}

void SlugsDataSensorView::setActiveUAS(UASInterface* uas){
    activeUAS = uas;
}

#ifdef MAVLINK_ENABLED_SLUGS

void SlugsDataSensorView::slugsGlobalPositionChanged(UASInterface *uas,
                                                     double lat,
                                                     double lon,
                                                     double alt,
                                                     quint64 time) {
 Q_UNUSED(uas);
 Q_UNUSED(time);

 ui->m_GpsLatitude->setText(QString::number(lat));
 ui->m_GpsLongitude->setText(QString::number(lon));
 ui->m_GpsHeight->setText(QString::number(alt));
}


void SlugsDataSensorView::slugLocalPositionChanged(UASInterface* uas,
                                                   double x,
                                                   double y,
                                                   double z,
                                                   quint64 time) {
  Q_UNUSED(uas);
  Q_UNUSED(time);

  ui->ed_x->setPlainText(QString::number(x));
  ui->ed_y->setPlainText(QString::number(y));
  ui->ed_z->setPlainText(QString::number(z));

}

void SlugsDataSensorView::slugSpeedLocalPositionChanged(UASInterface* uas,
                                                        double vx,
                                                        double vy,
                                                        double vz,
                                                        quint64 time) {
    Q_UNUSED( uas);
  Q_UNUSED(time);

  ui->ed_vx->setPlainText(QString::number(vx));
  ui->ed_vy->setPlainText(QString::number(vy));
  ui->ed_vz->setPlainText(QString::number(vz));

}

void SlugsDataSensorView::slugAttitudeChanged(UASInterface* uas,
                                              double slugroll,
                                              double slugpitch,
                                              double slugyaw,
                                              quint64 time)
{
    Q_UNUSED( uas);
    Q_UNUSED(time);

    ui->m_Roll->setPlainText(QString::number(slugroll));
    ui->m_Pitch->setPlainText(QString::number(slugpitch));
    ui->m_Yaw->setPlainText(QString::number(slugyaw));

}


void SlugsDataSensorView::slugsSensorBiasChanged(int systemId,
                                                 const mavlink_sensor_bias_t& sensorBias){
     Q_UNUSED( systemId);

  ui->m_AxBiases->setText(QString::number(sensorBias.axBias));
  ui->m_AyBiases->setText(QString::number(sensorBias.ayBias));
  ui->m_AzBiases->setText(QString::number(sensorBias.azBias));
  ui->m_GxBiases->setText(QString::number(sensorBias.gxBias));
  ui->m_GyBiases->setText(QString::number(sensorBias.gyBias));
  ui->m_GzBiases->setText(QString::number(sensorBias.gzBias));

}

void SlugsDataSensorView::slugsDiagnosticMessageChanged(int systemId,
                                                        const mavlink_diagnostic_t& diagnostic){
    Q_UNUSED(systemId);

  ui->m_Fl1->setText(QString::number(diagnostic.diagFl1));
  ui->m_Fl2->setText(QString::number(diagnostic.diagFl2));
  ui->m_Fl3->setText(QString::number(diagnostic.diagFl2));

  ui->m_Sh1->setText(QString::number(diagnostic.diagSh1));
  ui->m_Sh2->setText(QString::number(diagnostic.diagSh2));
  ui->m_Sh3->setText(QString::number(diagnostic.diagSh3));
}


void SlugsDataSensorView::slugsCpuLoadChanged(int systemId,
                                              const mavlink_cpu_load_t& cpuLoad){
     Q_UNUSED(systemId);
  ui->ed_sens->setText(QString::number(cpuLoad.sensLoad));
  ui->ed_control->setText(QString::number(cpuLoad.ctrlLoad));
  ui->ed_batvolt->setText(QString::number(cpuLoad.batVolt));
}

void SlugsDataSensorView::slugsNavegationChanged(int systemId,
                                                 const mavlink_slugs_navigation_t& slugsNavigation){
     Q_UNUSED(systemId);
  ui->m_Um->setText(QString::number(slugsNavigation.u_m));
  ui->m_PhiC->setText(QString::number(slugsNavigation.phi_c));
  ui->m_PitchC->setText(QString::number(slugsNavigation.theta_c));
  ui->m_PsidC->setText(QString::number(slugsNavigation.psiDot_c));
  ui->m_AyBody->setText(QString::number(slugsNavigation.ay_body));
  ui->m_TotRun->setText(QString::number(slugsNavigation.totalDist));
  ui->m_DistToGo->setText(QString::number(slugsNavigation.dist2Go));
  ui->m_FromWP->setText(QString::number(slugsNavigation.fromWP));
  ui->m_ToWP->setText(QString::number(slugsNavigation.toWP));
}



void SlugsDataSensorView::slugsDataLogChanged(int systemId,
                                              const mavlink_data_log_t& dataLog){
     Q_UNUSED(systemId);
  ui->m_logFl1->setText(QString::number(dataLog.fl_1));
  ui->m_logFl2->setText(QString::number(dataLog.fl_2));
  ui->m_logFl3->setText(QString::number(dataLog.fl_3));
  ui->m_logFl4->setText(QString::number(dataLog.fl_4));
  ui->m_logFl5->setText(QString::number(dataLog.fl_5));
  ui->m_logFl6->setText(QString::number(dataLog.fl_6));
}

void SlugsDataSensorView::slugsPWMChanged(int systemId,
                                          const mavlink_pwm_commands_t& pwmCommands){
       Q_UNUSED(systemId);
  ui->m_pwmThro->setText(QString::number(pwmCommands.dt_c));
  ui->m_pwmAile->setText(QString::number(pwmCommands.dla_c));
  ui->m_pwmElev->setText(QString::number(pwmCommands.dle_c));
  ui->m_pwmRudd->setText(QString::number(pwmCommands.dr_c));

  ui->m_pwmThroTrim->setText(QString::number(pwmCommands.dre_c));
  ui->m_pwmAileTrim->setText(QString::number(pwmCommands.dlf_c));
  ui->m_pwmElevTrim->setText(QString::number(pwmCommands.drf_c));
  ui->m_pwmRuddTrim->setText(QString::number(pwmCommands.aux1));

}

void SlugsDataSensorView::slugsFilteredDataChanged(int systemId,
                                                   const mavlink_filtered_data_t& filteredData){
    Q_UNUSED(systemId);
  ui->m_Axf->setText(QString::number(filteredData.aX));
  ui->m_Ayf->setText(QString::number(filteredData.aY));
  ui->m_Azf->setText(QString::number(filteredData.aZ));
  ui->m_Gxf->setText(QString::number(filteredData.gX));
  ui->m_Gyf->setText(QString::number(filteredData.gY));
  ui->m_Gzf->setText(QString::number(filteredData.gZ));
  ui->m_Mxf->setText(QString::number(filteredData.mX));
  ui->m_Myf->setText(QString::number(filteredData.mY));
  ui->m_Mzf->setText(QString::number(filteredData.mZ));
}

void SlugsDataSensorView::slugsGPSDateTimeChanged(int systemId,
                                                  const mavlink_gps_date_time_t& gpsDateTime){
    Q_UNUSED(systemId);
  ui->m_GpsDate->setText(QString::number(gpsDateTime.month) + "/" +
                         QString::number(gpsDateTime.day) + "/" +
                         QString::number(gpsDateTime.year));

  ui->m_GpsTime->setText(QString::number(gpsDateTime.hour) + ":" +
                         QString::number(gpsDateTime.min) + ":" +
                         QString::number(gpsDateTime.sec));

  ui->m_GpsSat->setText(QString::number(gpsDateTime.visSat));
}



#endif // MAVLINK_ENABLED_SLUGS
