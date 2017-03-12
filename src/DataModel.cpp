#include <Particle.h>

#include "DataModel.h"

DataModel::DataModel(int loginTimeSec, int logoutTimeSec) {
  login(loginTimeSec);
  logout(logoutTimeSec);
}

void DataModel::login() {
  DataModel::login(Time.now());
}
void DataModel::login(int loginTimeSec) {
  if (loginTimeSec > 1483228800 && loginTimeSec < 4102444800) {
      loginTimeSec_ = loginTimeSec;
      EEPROM.put(ENTER_ADDR, loginTimeSec_);
  }
}
int DataModel::getLoginTimeSec() {
  return loginTimeSec_;
}

void DataModel::logout() {
  logout(Time.now());
}
void DataModel::logout(int logoutTimeSec) {
  if (logoutTimeSec > 1483228800 && logoutTimeSec < 4102444800) {
      logoutTimeSec_ = logoutTimeSec;
      EEPROM.put(EXIT_ADDR, logoutTimeSec_);
  }
}
int DataModel::getLogoutTimeSec() {
  return logoutTimeSec_;
}

bool DataModel::atWork() {
  return loginTimeSec_ > logoutTimeSec_;
}

int DataModel::getAtWorkTimeSec() {
  return Time.now() - loginTimeSec_;
}
